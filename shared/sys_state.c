#include "sys_state.h"
#include "notify.h"
#include <stdio.h>

int sceSystemStateMgrGetCurrentState(void);

bool isPatchAllowed_(const char* user)
{
    // https://github.com/OSM-Made/Orbis-Suite/blob/67be2f38902df5010144f17f35d1a823ebb4238f/Playstation/OrbisAPIDaemon/SystemMonitor.cpp#L9
    // https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain/blob/0a1aaf9dd4a92695538bdeb09fb056d06dd11725/include/orbis/_types/sys_service.h#L108
    enum
    {
        INVALID_STATE = 0,
        INITIALIZING = 10,
        SHUTDOWN_ON_GOING = 100,
        POWER_SAVING = 200,
        SUSPEND_ON_GOING = 300,
        MAIN_ON_STANDBY = 500,
        WORKING = 1000,
    };
    const int r = sceSystemStateMgrGetCurrentState();
    notify_(false, user, "sceSystemStateMgrGetCurrentState() %d 0x%08x\n", r, r);
    switch (r)
    {
        case INITIALIZING:
        case SHUTDOWN_ON_GOING:
        case POWER_SAVING:
        case SUSPEND_ON_GOING:
        case MAIN_ON_STANDBY:
        {
            return false;
        }
        default:
        {
            break;
        }
    }
    return true;
}
