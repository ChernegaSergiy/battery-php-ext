/**
 * windows_battery.c - Windows battery information implementation
 *
 * Uses GetSystemPowerStatus API
 */

#include "include/battery_common.h"

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void windows_battery_get_details(zval *return_value)
{
    zval batteries;
    array_init(&batteries);

    SYSTEM_POWER_STATUS sps;
    if (GetSystemPowerStatus(&sps)) {
        zval bat;
        array_init(&bat);

        add_assoc_string(&bat, BATTERY_KEY_NAME, "System Battery");

        /* Capacity level */
        int has_level = (sps.BatteryLifePercent != 255);
        if (has_level) {
            add_assoc_long(&bat, BATTERY_KEY_LEVEL, (zend_long)sps.BatteryLifePercent);
        } else {
            add_assoc_null(&bat, BATTERY_KEY_LEVEL);
        }

        /* Charging status (ACLineStatus: 0=Offline, 1=Online, 255=Unknown) */
        int has_status = (sps.ACLineStatus != 255);
        if (has_status) {
            int charging = (sps.ACLineStatus == 1) ? 1 : 0;
            add_assoc_bool(&bat, BATTERY_KEY_CHARGING, charging);
            add_assoc_string(&bat, BATTERY_KEY_STATUS,
                charging ? BATTERY_STATUS_CHARGING : BATTERY_STATUS_DISCHARGING);
        } else {
            add_assoc_null(&bat, BATTERY_KEY_CHARGING);
            add_assoc_null(&bat, BATTERY_KEY_STATUS);
        }

        add_next_index_zval(&batteries, &bat);

        /* Add batteries array first (consistent with other platforms) */
        add_assoc_zval(return_value, BATTERY_KEY_BATTERIES, &batteries);

        /* Add aggregated values */
        if (has_level) {
            add_assoc_long(return_value, BATTERY_KEY_LEVEL, (zend_long)sps.BatteryLifePercent);
        } else {
            add_assoc_null(return_value, BATTERY_KEY_LEVEL);
        }

        if (has_status) {
            int charging = (sps.ACLineStatus == 1) ? 1 : 0;
            add_assoc_bool(return_value, BATTERY_KEY_CHARGING, charging);
            add_assoc_string(return_value, BATTERY_KEY_STATUS,
                charging ? BATTERY_STATUS_CHARGING : BATTERY_STATUS_DISCHARGING);
        } else {
            add_assoc_null(return_value, BATTERY_KEY_CHARGING);
            add_assoc_null(return_value, BATTERY_KEY_STATUS);
        }
    } else {
        add_assoc_zval(return_value, BATTERY_KEY_BATTERIES, &batteries);
        BATTERY_ADD_NULL_INFO(return_value);
    }
}

#else

void windows_battery_get_details(zval *return_value)
{
    BATTERY_INIT_EMPTY(return_value);
}

#endif

