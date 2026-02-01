#include <jni.h>
#include <stdio.h>
#include <string.h>

/* These functions are implemented in android_battery.c */
int android_battery_level(void);
int android_battery_is_charging(void);

JNIEXPORT jstring JNICALL
Java_com_example_batterytest_MainActivity_getBatteryInfoFromJNI(
        JNIEnv* env,
        jobject thiz) {
    
    int level = android_battery_level();
    int charging = android_battery_is_charging();
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), 
             "Battery Info (via JNI):\n\nLevel: %d%%\nCharging: %s", 
             level, charging ? "Yes" : "No");

    return (*env)->NewStringUTF(env, buffer);
}
