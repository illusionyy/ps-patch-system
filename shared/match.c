#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "macro.h"
#include "stringid.h"

int match_line_in_file(const char* filename, const char* input)
{
    FILE* f = fopen(filename, "rb");
    if (!f)
    {
        printf(FILE_FUNC_LINE ": %s not found nya\n", filename);
        return -__LINE__;
    }

    char line[1024 + 1] = {};
    int found = 0;
    const size_t input_l = strlen(input);
    const uint32_t input_h = sid_len(input, input_l);

    while (fgets(line, _countof_1(line), f))
    {
        char* p = line;
        while (*p == ' ' || *p == '\t')
        {
            p++;
        }

        if (*p == '#')
        {
            continue;
        }

        puts(p);

        if (sid_len(p, input_l) == input_h)
        {
            found = 1;
            break;
        }
    }

    fclose(f);
    return found;
}
