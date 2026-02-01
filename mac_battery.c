/**
 * mac_battery.c - macOS battery information implementation
 *
 * Uses IOKit Power Sources API
 */

#include "include/battery_common.h"

#if defined(__APPLE__)

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/ps/IOPowerSources.h>
#include <IOKit/ps/IOPSKeys.h>

void mac_battery_get_details(zval *return_value)
{
    zval batteries;
    array_init(&batteries);

    int total_level = 0;
    int battery_count = 0;
    int is_charging = 0;

    CFTypeRef blob = IOPSCopyPowerSourcesInfo();
    if (blob) {
        CFArrayRef list = IOPSCopyPowerSourcesList(blob);
        if (list) {
            long source_count = CFArrayGetCount(list);

            for (int i = 0; i < source_count; i++) {
                CFDictionaryRef ps = IOPSGetPowerSourceDescription(
                    blob, CFArrayGetValueAtIndex(list, i));
                if (!ps) {
                    continue;
                }

                /* Filter for internal batteries only */
                CFStringRef type = CFDictionaryGetValue(ps, CFSTR(kIOPSTypeKey));
                if (type && CFStringCompare(type, CFSTR(kIOPSInternalBatteryType), 0)
                    != kCFCompareEqualTo) {
                    continue;
                }

                zval bat;
                array_init(&bat);

                /* Battery name */
                CFStringRef name = CFDictionaryGetValue(ps, CFSTR(kIOPSNameKey));
                if (name) {
                    char buf[64];
                    if (CFStringGetCString(name, buf, sizeof(buf), kCFStringEncodingUTF8)) {
                        add_assoc_string(&bat, BATTERY_KEY_NAME, buf);
                    }
                }

                /* Capacity level */
                CFNumberRef cap = CFDictionaryGetValue(ps, CFSTR(kIOPSCurrentCapacityKey));
                if (cap) {
                    int level = -1;
                    CFNumberGetValue(cap, kCFNumberIntType, &level);
                    add_assoc_long(&bat, BATTERY_KEY_LEVEL, level);
                    total_level += level;
                    battery_count++;
                } else {
                    add_assoc_null(&bat, BATTERY_KEY_LEVEL);
                }

                /* Charging status */
                CFStringRef state = CFDictionaryGetValue(ps, CFSTR(kIOPSPowerSourceStateKey));
                if (state) {
                    int charging = (CFStringCompare(state, CFSTR(kIOPSACPowerValue), 0)
                                    == kCFCompareEqualTo) ? 1 : 0;
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
            CFRelease(list);
        }
        CFRelease(blob);
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

void mac_battery_get_details(zval *return_value)
{
    BATTERY_INIT_EMPTY(return_value);
}

#endif

