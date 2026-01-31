#if defined(__ANDROID__)
#include <jni.h>
#include <stdio.h>

static JavaVM *g_jvm = NULL;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    g_jvm = vm;
    return JNI_VERSION_1_6;
}

static JNIEnv *get_jni_env(void)
{
    if (!g_jvm) return NULL;
    JNIEnv *env = NULL;
    if ((*g_jvm)->GetEnv(g_jvm, (void **)&env, JNI_VERSION_1_6) != JNI_OK) {
        if ((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != 0) return NULL;
    }
    return env;
}

int android_battery_level(void)
{
    JNIEnv *env = get_jni_env();
    if (!env) return -1;

    jclass cls = (*env)->FindClass(env, "com/example/Battery");
    if (!cls) return -1;

    jmethodID mid = (*env)->GetStaticMethodID(env, cls, "getLevel", "()I");
    if (!mid) return -1;

    jint level = (*env)->CallStaticIntMethod(env, cls, mid);
    return (int)level;
}

int android_battery_is_charging(void)
{
    JNIEnv *env = get_jni_env();
    if (!env) return -1;

    jclass cls = (*env)->FindClass(env, "com/example/Battery");
    if (!cls) return -1;

    jmethodID mid = (*env)->GetStaticMethodID(env, cls, "isCharging", "()Z");
    if (!mid) return -1;

    jboolean res = (*env)->CallStaticBooleanMethod(env, cls, mid);
    return res ? 1 : 0;
}

#else
int android_battery_level(void) { (void)0; return -1; }
int android_battery_is_charging(void) { (void)0; return -1; }
#endif
