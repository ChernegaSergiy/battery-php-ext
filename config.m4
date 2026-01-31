PHP_ARG_ENABLE(battery_info, whether to enable the battery_info extension,
[  --enable-battery_info   Enable battery_info support])

if test "$PHP_BATTERY_INFO" != "no"; then
  PHP_NEW_EXTENSION(battery_info, php_battery.c linux_battery.c mac_battery.c windows_battery.c android_battery.c, $ext_shared)
fi
