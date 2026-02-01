/**
 * linux_battery.c - Linux battery information implementation
 *
 * Reads battery data from /sys/class/power_supply/
 */

#define _GNU_SOURCE
#include "include/battery_common.h"

#if defined(__linux__) || defined(__ANDROID__)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>

#define POWER_SUPPLY_PATH "/sys/class/power_supply"

/**
 * Read integer value from sysfs file
 */
static int read_int_file(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) {
        return -1;
    }

    int value = -1;
    if (fscanf(f, "%d", &value) != 1) {
        value = -1;
    }
    fclose(f);

    return value;
}

/**
 * Read charging status from sysfs file
 *
 * @return 1=charging, 0=discharging, -1=unknown
 */
static int read_status_file(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) {
        return -1;
    }

    char buf[64] = {0};
    if (fscanf(f, "%63s", buf) != 1) {
        fclose(f);
        return -1;
    }
    fclose(f);

    if (strcasecmp(buf, "Charging") == 0 || strcasecmp(buf, "Full") == 0) {
        return 1;
    }
    if (strcasecmp(buf, "Discharging") == 0) {
        return 0;
    }

    return -1;
}

/**
 * Check if power supply is a battery type
 */
static int is_battery_type(const char *base, const char *name)
{
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s/type", base, name);

    FILE *f = fopen(path, "r");
    if (!f) {
        return 0;
    }

    char buf[64] = {0};
    int result = 0;
    if (fscanf(f, "%63s", buf) == 1 && strcasecmp(buf, "Battery") == 0) {
        result = 1;
    }
    fclose(f);

    return result;
}

void linux_battery_get_details(zval *return_value)
{
    zval batteries;
    array_init(&batteries);

    int total_level = 0;
    int battery_count = 0;
    int is_charging = 0;

    DIR *dir = opendir(POWER_SUPPLY_PATH);
    if (dir) {
        struct dirent *entry;
        char path[PATH_MAX];

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] == '.') {
                continue;
            }
            if (!is_battery_type(POWER_SUPPLY_PATH, entry->d_name)) {
                continue;
            }

            zval bat;
            array_init(&bat);

            /* Battery name */
            add_assoc_string(&bat, BATTERY_KEY_NAME, entry->d_name);

            /* Capacity level */
            snprintf(path, sizeof(path), "%s/%s/capacity",
                     POWER_SUPPLY_PATH, entry->d_name);
            int level = read_int_file(path);
            if (level >= 0) {
                add_assoc_long(&bat, BATTERY_KEY_LEVEL, level);
                total_level += level;
                battery_count++;
            } else {
                add_assoc_null(&bat, BATTERY_KEY_LEVEL);
            }

            /* Charging status */
            snprintf(path, sizeof(path), "%s/%s/status",
                     POWER_SUPPLY_PATH, entry->d_name);
            int charging = read_status_file(path);
            if (charging >= 0) {
                add_assoc_bool(&bat, BATTERY_KEY_CHARGING, charging);
                add_assoc_string(&bat, BATTERY_KEY_STATUS,
                    charging ? BATTERY_STATUS_CHARGING : BATTERY_STATUS_DISCHARGING);
                if (charging) {
                    is_charging = 1;
                }
            } else {
                add_assoc_null(&bat, BATTERY_KEY_CHARGING);
                add_assoc_null(&bat, BATTERY_KEY_STATUS);
            }

            add_next_index_zval(&batteries, &bat);
        }
        closedir(dir);
    }

    /* Add batteries array */
    add_assoc_zval(return_value, BATTERY_KEY_BATTERIES, &batteries);

    /* Add aggregated values */
    if (battery_count > 0) {
        BATTERY_ADD_INFO(return_value, total_level / battery_count, is_charging);
    } else {
        BATTERY_ADD_NULL_INFO(return_value);
    }
}

#else

void linux_battery_get_details(zval *return_value)
{
    BATTERY_INIT_EMPTY(return_value);
}

#endif
