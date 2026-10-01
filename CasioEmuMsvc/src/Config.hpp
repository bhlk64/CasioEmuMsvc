#pragma once
#include "Containers/ConcurrentObject.h"
#include <cstdint>
#include <cstdio>
#include <exception>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#ifdef __GNUG__
#define FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define FUNCTION_NAME __func__
#endif

#ifndef __debugbreak
    #if defined(_MSC_VER)
    #elif defined(__GNUC__) || defined(__clang__)
        #define __debugbreak __builtin_trap
    #else
        #define __debugbreak() ((void)0)
    #endif
#endif

#define NOOP() ((void)0)

#define ENABLE_CRASH_CHECK

#ifdef ENABLE_CRASH_CHECK

#if defined(__ANDROID__)

#include <android/log.h>

#define PANIC(...) \
  do { \
    __android_log_print(ANDROID_LOG_ERROR, "PANIC", __VA_ARGS__); \
  } while (0)

#else

#ifndef PANIC
#define PANIC(...) \
  do { \
    printf(__VA_ARGS__); \
    __debugbreak(); \
  } while (0)
#endif

#endif

#else

#ifndef PANIC
#define PANIC(...) 0
#endif

#endif

#include <cstdio>
#include <cstdarg>

#if defined(__ANDROID__)
#define EMULATOR_LOG_FILE \
  "/storage/emulated/0/Android/data/com.tele.u8emulator/files/emulator_printf.txt"
#else
#define EMULATOR_LOG_FILE "emulator_printf.txt"
#endif

static void log_printf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

  // printf như cũ
  va_list copy;
  va_copy(copy, args);
  vprintf(fmt, copy);
  va_end(copy);

  // Ghi thêm vào file
  FILE* fp = fopen(EMULATOR_LOG_FILE, "a");
  if (fp) {
    va_copy(copy, args);
    vfprintf(fp, fmt, copy);
    va_end(copy);
    fflush(fp);
    fclose(fp);
  }

  va_end(args);
}

#define printf(...) log_printf(__VA_ARGS__)

#define LOCK(x) \
	std::lock_guard<std::mutex> lock_##x{x};

// Enable debug feature

#define DBG

#ifdef min
#undef min
#endif 
#ifdef max
#undef max
#endif 

// #define SINGLE_WINDOW
#if !defined(SINGLE_WINDOW) && defined(__ANDROID__)
#define SINGLE_WINDOW
#endif

#if defined(_MSC_VER) || (defined(__clang__) && defined(_WIN32))
#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)
#elif defined(__clang__) || defined(__GNUC__)
#define DLLEXPORT __attribute__((visibility("default")))
#define DLLIMPORT
#else
#define DLLEXPORT
#define DLLIMPORT
#endif

#define PROP(x)                                  \
public:                                          \
	virtual decltype(x) Get##x##() { return x; } \
	virtual void Set##x##(decltype(x) a) { x = a; }

#define PROPABS(y, x)         \
public:                       \
	virtual y Get##x##() = 0; \
	virtual void Set##x##(y a) = 0;

#include <git_info.h>

#define EMULATOR_VERSION GIT_COMMIT_HASH

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__) && !defined(DISABLE_SENTRY)
#define ENABLE_SENTRY
#define SENTRY_BUILD_STATIC 1
#endif

#define DISCORD_APP_ID "1494244788055179344"
