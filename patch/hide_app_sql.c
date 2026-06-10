
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(__ORBIS__)
#include <sqlite3.h>
#endif

#include "../shared/notify.h"
#include "../shared/debug.h"
#include "../shared/macro.h"

#if defined(__ORBIS__)
static int process_table(void* db_ptr, int argc, char** argv, char** col_names)
{
    (void)argc;
    (void)col_names;
    sqlite3* db = (sqlite3*)db_ptr;
    const char* tbl = argv[0];
    static const char* hide_id[] = {
        "NPXS20979",  // PlayStation Store
        "NPXS20108",  // What's New?
        "NPXS20114",  // PlayStation Now
        "NPXS20107",  // PlayStation Video - My Videos
        "NPXS20105",  // Live from PlayStation
        "NPXS20106",  // Music Unlimited
        "NPXS20118",  // Share Play
    };

    for (size_t i = 0; i < _countof(hide_id); i++)
    {
        char sql[add_1(512)] = {0};
        snprintf(sql, _countof_1(sql), "UPDATE \"%s\" SET visible = 0 WHERE titleId = '%s';", tbl, hide_id[i]);

        char* err = NULL;
        int rc = sqlite3_exec(db, sql, NULL, NULL, &err);
        if (rc != SQLITE_OK)
        {
            debugf("[WARN] %s on table %s: %s\n",
                   hide_id[i],
                   tbl,
                   err);
            sqlite3_free(err);
            return rc;
        }
        else
        {
            int changes = sqlite3_changes(db);
            if (changes > 0)
            {
                debugf("[OK] %s -> %s  (%d row(s) updated)\n",
                       tbl,
                       hide_id[i],
                       changes);
            }
        }
    }
    return 0;
}
#endif

int hide_apps(const char* db_path)
{
#if defined(__ORBIS__)
    sqlite3* db = NULL;
    int rc;
    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK)
    {
        debugf("Cannot open database '%s': %s\n",
               db_path,
               sqlite3_errmsg(db));
        sqlite3_close(db);
        return -(__LINE__);
    }
    debugf("Opened '%s' successfully.\n\n", db_path);

    const char* find_tables =
        "SELECT name FROM sqlite_master "
        "WHERE type = 'table' AND name LIKE 'tbl_appbrowse_%' "
        "ORDER BY name;";

    char* err = NULL;
    rc = sqlite3_exec(db, find_tables, process_table, db, &err);
    if (rc != SQLITE_OK)
    {
        const char* hint = rc == SQLITE_BUSY || rc == SQLITE_LOCKED ? "Try running payload when no apps are running" : "";
        notify("Failed to query table list: %s.\n%s\nError code: %d\n", err, hint, rc);
        sqlite3_free(err);
        sqlite3_close(db);
        return -(__LINE__);
    }

    debugf("Done.\n");
    sqlite3_close(db);
#endif
    return 0;
}
