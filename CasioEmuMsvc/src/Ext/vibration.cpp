//
// Created by 15874 on 2024/8/9.
//
#include "vibration.h"
bool setting_DisableVibration = false;
#ifdef __ANDROID__
#include <SDL.h>
#include <jni.h>

void Vibration::vibrate(long milliseconds) {
	if (setting_DisableVibration)
		return;
    // 鑾峰彇JNI鐜
    JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
    if (env == NULL) {
        return;
    }

    // 鑾峰彇SDLActivity绫?
    jclass activityClass = env->FindClass("com/tele/u8emulator/Game");
    if (activityClass == NULL) {
        return;
    }

    // 鑾峰彇nativeVibrate闈欐€佹柟娉曠殑ID
    jmethodID vibrateMethod = env->GetStaticMethodID(activityClass, "nativeVibrate", "(J)V");
    if (vibrateMethod == NULL) {
        return;
    }

    // 璋冪敤nativeVibrate闈欐€佹柟娉?
    env->CallStaticVoidMethod(activityClass, vibrateMethod, (jlong)milliseconds);
}


extern "C"
{
	JNIEXPORT void JNICALL Java_com_tele_u8emulator_Game_nativeVibrate(JNIEnv* env, jclass cls, jlong milliseconds) {
		// 鑾峰彇SDLActivity绫?
		jclass activityClass = env->FindClass("com/tele/u8emulator/Game");
		if (activityClass == NULL) {
			return;
		}

		// 鑾峰彇nativeVibrate闈欐€佹柟娉曠殑ID
		jmethodID vibrateMethod = env->GetStaticMethodID(activityClass, "nativeVibrate", "(J)V");
		if (vibrateMethod == NULL) {
			return;
		}

		// 璋冪敤nativeVibrate闈欐€佹柟娉?
		env->CallStaticVoidMethod(activityClass, vibrateMethod, milliseconds);
	}
}
#elif defined(IOS)
#include "iOSNativeBridge.h"

void Vibration::vibrate(long milliseconds)
{
    if (setting_DisableVibration)
        return;

    nativeVibrate(milliseconds);
}
#else
void Vibration::vibrate(long milliseconds) {
}
#endif