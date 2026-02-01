/**
 * android_battery.c - Android battery information implementation
 *
 * Uses JNI to call Java Battery helper class
 */

#include "include/battery_common.h"

#if defined(__ANDROID__)

#include <jni.h>
#include <stdio.h>

static JavaVM *g_jvm = NULL;

/**
 * JNI_OnLoad - Store JVM reference when library is loaded
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    g_jvm = vm;
    return JNI_VERSION_1_6;
}

/**
 * Get JNI environment for current thread
 */
static JNIEnv *get_jni_env(void)
{
    if (!g_jvm) {
        return NULL;
    }

    JNIEnv *env = NULL;
    if ((*g_jvm)->GetEnv(g_jvm, (void **)&env, JNI_VERSION_1_6) != JNI_OK) {
        if ((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != 0) {
            return NULL;
        }
    }

    return env;
}

/**
 * Get battery level via JNI
 */
static int get_level_jni(JNIEnv *env)
{
    jclass cls = (*env)->FindClass(env, "com/example/Battery");
    if (!cls) {
        return -1;
    }

    jmethodID mid = (*env)->GetStaticMethodID(env, cls, "getLevel", "()I");
    if (!mid) {
        return -1;
    }

    return (int)(*env)->CallStaticIntMethod(env, cls, mid);
}

/**
 * Get charging status via JNI
 */
static int get_charging_jni(JNIEnv *env)
{
    jclass cls = (*env)->FindClass(env, "com/example/Battery");
    if (!cls) {
        return -1;
    }

    jmethodID mid = (*env)->GetStaticMethodID(env, cls, "isCharging", "()Z");
    if (!mid) {
        return -1;
    }

    return (*env)->CallStaticBooleanMethod(env, cls, mid) ? 1 : 0;
}

void android_battery_get_details(zval *return_value)
{
    zval batteries;
    array_init(&batteries);

    JNIEnv *env = get_jni_env();
    if (env) {
        int level = get_level_jni(env);
        int charging = get_charging_jni(env);

        zval bat;
        array_init(&bat);

        add_assoc_string(&bat, BATTERY_KEY_NAME, "main");

        /* Capacity level */
        if (level >= 0) {
            add_assoc_long(&bat, BATTERY_KEY_LEVEL, level);
        } else {
            add_assoc_null(&bat, BATTERY_KEY_LEVEL);
        }

        /* Charging status */
        if (charging >= 0) {
            add_assoc_bool(&bat, BATTERY_KEY_CHARGING, charging);
            add_assoc_string(&bat, BATTERY_KEY_STATUS,
                charging ? BATTERY_STATUS_CHARGING : BATTERY_STATUS_DISCHARGING);
        } else {
            add_assoc_null(&bat, BATTERY_KEY_CHARGING);
            add_assoc_null(&bat, BATTERY_KEY_STATUS);
        }

        add_next_index_zval(&batteries, &bat);

        /* Add batteries array */
        add_assoc_zval(return_value, BATTERY_KEY_BATTERIES, &batteries);

        /* Add aggregated values */
        if (level >= 0 && charging >= 0) {
            BATTERY_ADD_INFO(return_value, level, charging);
        } else if (level >= 0) {
            add_assoc_long(return_value, BATTERY_KEY_LEVEL, level);
            add_assoc_null(return_value, BATTERY_KEY_CHARGING);
            add_assoc_null(return_value, BATTERY_KEY_STATUS);
        } else {
            BATTERY_ADD_NULL_INFO(return_value);
        }
    } else {
        /* No JNI environment available */
        add_assoc_zval(return_value, BATTERY_KEY_BATTERIES, &batteries);
        BATTERY_ADD_NULL_INFO(return_value);
    }
}

/**
 * Public API for native-lib.c in APK
 */
int android_battery_level(void)
{
    JNIEnv *env = get_jni_env();
    return env ? get_level_jni(env) : -1;
}

int android_battery_is_charging(void)
{
    JNIEnv *env = get_jni_env();
    return env ? get_charging_jni(env) : -1;
}

#else

void android_battery_get_details(zval *return_value)
{
    BATTERY_INIT_EMPTY(return_value);
}

int android_battery_level(void)
{
    return -1;
}

int android_battery_is_charging(void)
{
    return -1;
}

#endif
