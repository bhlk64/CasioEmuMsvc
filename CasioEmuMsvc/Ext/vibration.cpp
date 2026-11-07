
//
// Created by 15874 on 2024/8/9.
//

#include "vibration.h"

bool setting_DisableVibration = false;

#ifdef __ANDROID__

#include <jni.h>
#include <mutex>

static JavaVM* g_vm = nullptr;
static jclass g_gameClass = nullptr;
static jmethodID g_vibrateMethod = nullptr;

static std::mutex g_vibrationMutex;

// Called automatically when this native library is loaded.
extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
    g_vm = vm;
    return JNI_VERSION_1_6;
}

// Called from Java on a Java-originated thread.
// Cache the Game class and nativeVibrate method.
extern "C"
JNIEXPORT void JNICALL
Java_com_tele_u8emulator_Game_initNativeVibration(
    JNIEnv* env,
    jclass cls
) {
    std::lock_guard<std::mutex> lock(g_vibrationMutex);

    jclass newClass = (jclass)env->NewGlobalRef(cls);

    if (!newClass)
        return;

    jmethodID newMethod = env->GetStaticMethodID(
        newClass,
        "nativeVibrate",
        "(J)V"
    );

    if (!newMethod) {
        if (env->ExceptionCheck())
            env->ExceptionClear();

        env->DeleteGlobalRef(newClass);
        return;
    }

    if (g_gameClass)
        env->DeleteGlobalRef(g_gameClass);

    g_gameClass = newClass;
    g_vibrateMethod = newMethod;
}

void Vibration::vibrate(long milliseconds) {
    if (setting_DisableVibration)
        return;

    if (!g_vm)
        return;

    JNIEnv* env = nullptr;
    bool attached = false;

    jint result = g_vm->GetEnv(
        (void**)&env,
        JNI_VERSION_1_6
    );

    if (result == JNI_EDETACHED) {
        if (g_vm->AttachCurrentThread(&env, nullptr) != JNI_OK)
            return;

        attached = true;
    }
    else if (result != JNI_OK) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(g_vibrationMutex);

        if (g_gameClass && g_vibrateMethod) {
            env->CallStaticVoidMethod(
                g_gameClass,
                g_vibrateMethod,
                (jlong)milliseconds
            );
        }

        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
    }

    if (attached)
        g_vm->DetachCurrentThread();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tele_u8emulator_Game_nativeVibrate(
    JNIEnv* env,
    jclass cls,
    jlong milliseconds
) {
    // Keep the existing Java native method implementation empty.
    // Java-side vibration handling should remain in Game.nativeVibrate().
}

#else

void Vibration::vibrate(long milliseconds) {
}

#endif
