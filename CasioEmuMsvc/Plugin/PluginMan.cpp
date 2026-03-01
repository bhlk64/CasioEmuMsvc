#include "PluginApi.h"

extern PluginApi* g_pluginapi;

#ifdef _WIN32

#include <Windows.h>

void LoadPlugins() {
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA("CasioEmuMsvc.Plugin.*.dll", &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        HMODULE hModule = LoadLibraryA(findData.cFileName);
        if (hModule) {
            auto load = (PluginLoad)GetProcAddress(hModule, "fPluginLoad");
            if (load) {
                load(g_pluginapi);
            }
        }
    } while (FindNextFileA(hFind, &findData));

    FindClose(hFind);
}

#endif


#ifdef __ANDROID__

#include <dlfcn.h>

void LoadPlugins() {

    void* handle = dlopen("libCasioEmuMsvc.Plugin.MyPlugin.so", RTLD_NOW);
    if (!handle)
        return;

    auto load = (PluginLoad)dlsym(handle, "fPluginLoad");
    if (load) {
        load(g_pluginapi);
    }
}

#endif