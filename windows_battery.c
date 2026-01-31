#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

int windows_battery_level(void)
{
#if defined(_WIN32)
    SYSTEM_POWER_STATUS sps;
    if (!GetSystemPowerStatus(&sps)) return -1;
    if (sps.BatteryLifePercent == 255) return -1;
    return (int)sps.BatteryLifePercent;
#else
    (void)0;
    return -1;
#endif
}

int windows_battery_is_charging(void)
{
#if defined(_WIN32)
    SYSTEM_POWER_STATUS sps;
    if (!GetSystemPowerStatus(&sps)) return -1;
    return sps.ACLineStatus == 1 ? 1 : 0;
#else
    (void)0;
    return -1;
#endif
}
