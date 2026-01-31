#if defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/ps/IOPowerSources.h>
#include <IOKit/ps/IOPSKeys.h>
#endif

int mac_battery_level(void)
{
#if defined(__APPLE__)
    CFTypeRef blob = IOPSCopyPowerSourcesInfo();
    if (!blob) return -1;
    CFArrayRef list = IOPSCopyPowerSourcesList(blob);
    if (!list) {
        CFRelease(blob);
        return -1;
    }
    if (CFArrayGetCount(list) == 0) {
        CFRelease(list);
        CFRelease(blob);
        return -1;
    }
    CFDictionaryRef ps = IOPSGetPowerSourceDescription(blob, CFArrayGetValueAtIndex(list, 0));
    if (!ps) {
        CFRelease(list);
        CFRelease(blob);
        return -1;
    }
    CFNumberRef num = CFDictionaryGetValue(ps, CFSTR(kIOPSCurrentCapacityKey));
    int v = -1;
    if (num) CFNumberGetValue(num, kCFNumberIntType, &v);
    CFRelease(list);
    CFRelease(blob);
    return v;
#else
    (void)0;
    return -1;
#endif
}

int mac_battery_is_charging(void)
{
#if defined(__APPLE__)
    CFTypeRef blob = IOPSCopyPowerSourcesInfo();
    if (!blob) return -1;
    CFArrayRef list = IOPSCopyPowerSourcesList(blob);
    if (!list) { CFRelease(blob); return -1; }
    if (CFArrayGetCount(list) == 0) { CFRelease(list); CFRelease(blob); return -1; }
    CFDictionaryRef ps = IOPSGetPowerSourceDescription(blob, CFArrayGetValueAtIndex(list, 0));
    if (!ps) { CFRelease(list); CFRelease(blob); return -1; }
    CFStringRef state = CFDictionaryGetValue(ps, CFSTR(kIOPSPowerSourceStateKey));
    int charging = -1;
    if (state) {
        if (CFStringCompare(state, CFSTR(kIOPSACPowerValue), 0) == kCFCompareEqualTo) charging = 1;
        else charging = 0;
    }
    CFRelease(list);
    CFRelease(blob);
    return charging;
#else
    (void)0;
    return -1;
#endif
}
