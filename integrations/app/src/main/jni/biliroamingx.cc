#include <jni.h>
#include <cstdlib>
#include <android/log.h>
#include <pthread.h>
#include <string.h>
#include "dobby.h"

#define LOG_TAG "BiliRoamingX"

#ifndef NDEBUG
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#else
#define LOGD(...) 0
#endif
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// public static native a(I)Ljava/lang/String;
JNIEXPORT jstring JNICALL Java_kofua_rr_a(JNIEnv *env, jclass clazz, jint value) {
    char buffer[20]; 
    snprintf(buffer, sizeof(buffer), "%d", value);
    return (*env)->NewStringUTF(env, buffer);
}

// public static native b(Ljava/lang/String;)Ljava/lang/String;
JNIEXPORT jstring JNICALL Java_kofua_rr_b(JNIEnv *env, jclass clazz, jstring input) {
    return (*env)->NewLocalRef(env, input);
}

void fake_exit(int status) {
    LOGI("Exit function fake success, status: %d", status);
    DobbyDestroy((void *) exit);
    pthread_exit(nullptr);
}

JNIEXPORT JNICALL extern "C"
jint JNI_OnLoad(JavaVM *vm, void *) {
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK)
        return JNI_ERR;
    if (DobbyHook((void *) exit,
                  reinterpret_cast<dobby_dummy_func_t>(fake_exit),
                  nullptr) == RS_SUCCESS) {
        LOGD("Exit function hook success");
    } else {
        LOGD("Exit function hook failed");
    }
    return JNI_VERSION_1_6;
}
