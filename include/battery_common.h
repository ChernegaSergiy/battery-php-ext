/**
 * battery_common.h - Common definitions for battery_info PHP extension
 *
 * This header provides unified declarations for all platform-specific
 * battery information functions.
 */

#ifndef BATTERY_COMMON_H
#define BATTERY_COMMON_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

/* Extension version */
#define BATTERY_INFO_VERSION "0.1"

/* Extension name */
#define BATTERY_INFO_EXTNAME "battery_info"

/**
 * Platform identifiers
 */
#define BATTERY_PLATFORM_LINUX   "linux"
#define BATTERY_PLATFORM_MACOS   "macos"
#define BATTERY_PLATFORM_WINDOWS "windows"
#define BATTERY_PLATFORM_ANDROID "android"
#define BATTERY_PLATFORM_UNKNOWN "unknown"

/**
 * Status strings
 */
#define BATTERY_STATUS_CHARGING    "charging"
#define BATTERY_STATUS_DISCHARGING "discharging"

/**
 * Array keys used in return values
 */
#define BATTERY_KEY_LEVEL     "level"
#define BATTERY_KEY_CHARGING  "charging"
#define BATTERY_KEY_STATUS    "status"
#define BATTERY_KEY_PLATFORM  "platform"
#define BATTERY_KEY_BATTERIES "batteries"
#define BATTERY_KEY_NAME      "name"

/**
 * Platform-specific battery detail functions
 *
 * Each function populates return_value with:
 *   - "batteries" (array) - list of individual batteries, each with:
 *       - "name" (string) - battery identifier
 *       - "level" (int|null) - 0-100 or null
 *       - "charging" (bool|null) - charging state
 *       - "status" (string|null) - "charging"/"discharging"
 *   - "level" (int|null) - aggregated level (average)
 *   - "charging" (bool|null) - aggregated charging state
 *   - "status" (string|null) - aggregated status string
 *
 * @param return_value Pre-initialized zval array to populate
 */
void linux_battery_get_details(zval *return_value);
void mac_battery_get_details(zval *return_value);
void windows_battery_get_details(zval *return_value);
void android_battery_get_details(zval *return_value);

/**
 * Helper macro to add null values for missing battery info
 */
#define BATTERY_ADD_NULL_INFO(rv) do { \
    add_assoc_null((rv), BATTERY_KEY_LEVEL); \
    add_assoc_null((rv), BATTERY_KEY_CHARGING); \
    add_assoc_null((rv), BATTERY_KEY_STATUS); \
} while (0)

/**
 * Helper macro to add battery info values
 */
#define BATTERY_ADD_INFO(rv, lvl, chrg) do { \
    add_assoc_long((rv), BATTERY_KEY_LEVEL, (lvl)); \
    add_assoc_bool((rv), BATTERY_KEY_CHARGING, (chrg)); \
    add_assoc_string((rv), BATTERY_KEY_STATUS, \
        (chrg) ? BATTERY_STATUS_CHARGING : BATTERY_STATUS_DISCHARGING); \
} while (0)

/**
 * Helper macro to initialize empty batteries array
 */
#define BATTERY_INIT_EMPTY(rv) do { \
    zval batteries; \
    array_init(&batteries); \
    add_assoc_zval((rv), BATTERY_KEY_BATTERIES, &batteries); \
    BATTERY_ADD_NULL_INFO(rv); \
} while (0)

#endif /* BATTERY_COMMON_H */
