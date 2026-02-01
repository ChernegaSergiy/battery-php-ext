/**
 * php_battery.c - Main PHP extension entry point for battery_info
 */

#include "include/battery_common.h"

/**
 * battery_info() - Returns battery information for the current platform
 *
 * @return array Battery information with keys: level, charging, status, platform, batteries
 */
PHP_FUNCTION(battery_info)
{
    const char *platform = BATTERY_PLATFORM_UNKNOWN;

    array_init(return_value);

#if defined(__ANDROID__)
    platform = BATTERY_PLATFORM_ANDROID;
    /* Try sysfs first (works on some Android devices) */
    linux_battery_get_details(return_value);

    /* If sysfs didn't find battery info, try JNI */
    zval *level_val = zend_hash_str_find(Z_ARRVAL_P(return_value),
                                          BATTERY_KEY_LEVEL,
                                          sizeof(BATTERY_KEY_LEVEL) - 1);
    if (level_val && Z_TYPE_P(level_val) == IS_NULL) {
        zval_ptr_dtor(return_value);
        array_init(return_value);
        android_battery_get_details(return_value);
    }

#elif defined(__linux__)
    platform = BATTERY_PLATFORM_LINUX;
    linux_battery_get_details(return_value);

#elif defined(__APPLE__)
    platform = BATTERY_PLATFORM_MACOS;
    mac_battery_get_details(return_value);

#elif defined(_WIN32)
    platform = BATTERY_PLATFORM_WINDOWS;
    windows_battery_get_details(return_value);

#else
    BATTERY_INIT_EMPTY(return_value);
#endif

    add_assoc_string(return_value, BATTERY_KEY_PLATFORM, (char *)platform);
}

/* Argument info for battery_info() */
ZEND_BEGIN_ARG_INFO(arginfo_battery_info, 0)
ZEND_END_ARG_INFO()

/* Function entry table */
static const zend_function_entry battery_functions[] = {
    ZEND_FE(battery_info, arginfo_battery_info)
    ZEND_FE_END
};

/* Module entry */
zend_module_entry battery_module_entry = {
    STANDARD_MODULE_HEADER,
    BATTERY_INFO_EXTNAME,
    battery_functions,
    NULL,  /* MINIT */
    NULL,  /* MSHUTDOWN */
    NULL,  /* RINIT */
    NULL,  /* RSHUTDOWN */
    NULL,  /* MINFO */
    BATTERY_INFO_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_BATTERY_INFO
ZEND_GET_MODULE(battery)
#endif