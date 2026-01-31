#include <php.h>

/* Platform-specific declarations */
int linux_battery_level(void);
int linux_battery_is_charging(void);

int mac_battery_level(void);
int mac_battery_is_charging(void);

int windows_battery_level(void);
int windows_battery_is_charging(void);

int android_battery_level(void);
int android_battery_is_charging(void);

PHP_FUNCTION(battery_info)
{
    array_init(return_value);

    int level = -1;
    int charging = -1;
    const char *platform = "unknown";

#if defined(__ANDROID__)
    platform = "android";
    level = android_battery_level();
    charging = android_battery_is_charging();
#elif defined(__linux__)
    platform = "linux";
    level = linux_battery_level();
    charging = linux_battery_is_charging();
#elif defined(__APPLE__)
    platform = "macos";
    level = mac_battery_level();
    charging = mac_battery_is_charging();
#elif defined(_WIN32)
    platform = "windows";
    level = windows_battery_level();
    charging = windows_battery_is_charging();
#endif

    if (level >= 0) {
        add_assoc_long(return_value, "level", (zend_long)level);
    } else {
        add_assoc_null(return_value, "level");
    }

    if (charging == 0 || charging == 1) {
        add_assoc_bool(return_value, "charging", charging);
        add_assoc_string(return_value, "status", charging ? "charging" : "discharging");
    } else {
        add_assoc_null(return_value, "charging");
        add_assoc_null(return_value, "status");
    }

    add_assoc_string(return_value, "platform", (char*)platform);
}

/* extension entry */
ZEND_BEGIN_ARG_INFO(arginfo_battery_info, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry battery_functions[] = {
    ZEND_FE(battery_info, arginfo_battery_info)
    ZEND_FE_END
};

zend_module_entry battery_module_entry = {
    STANDARD_MODULE_HEADER,
    "battery_info",
    battery_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "0.1",
    STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(battery)
