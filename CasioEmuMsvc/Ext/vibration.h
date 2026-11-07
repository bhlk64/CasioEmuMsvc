//
// Created by 15874 on 2024/8/9.
//
#pragma once

extern bool setting_DisableVibration;
extern void Vibration_SetJavaVM(JavaVM* vm);

class Vibration {
public:
    static void vibrate(long milliseconds);
};
