//
// Created by 15874 on 2024/8/9.
//
#pragma once
#ifdef __ANDROID__
#include <jni.h>
extern void Vibration_SetJavaVM(JavaVM* vm);
#endif

extern bool setting_DisableVibration;

class Vibration {
public:
    static void vibrate(long milliseconds);
};
