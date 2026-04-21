#include "PluginApi.h"
#include "PluginMan.h"
#include <iostream>
#include <fstream>

extern PluginApi* g_pluginapi;
std::string g_PluginLoadLog = "";
std::vector<PluginInfo> g_loadedPlugins;

#ifdef _WIN32
#include <Windows.h>
#include <string>

void LoadPlugins() {
  g_PluginLoadLog += "[INFO] Starting Windows Plugin Loader\n";
	// Load all plugins in the plugins directory
	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA("CasioEmuMsvc.Plugin.*.dll", &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
	  g_PluginLoadLog += "[INFO] No plugins found.\n";
		return;
	}
	do {
	  g_PluginLoadLog += "[INFO] Found: " + std::string(findData.cFileName) + "\n";
		HMODULE hModule = LoadLibraryA(findData.cFileName);
		if (hModule) {
			auto load = (PluginLoad)GetProcAddress(hModule, "fPluginLoad");
			if (load) {
				load(g_pluginapi);
				g_PluginLoadLog += "[SUCCESS] Loaded plugin: " + std::string(findData.cFileName) + "\n";
            } else {
                g_PluginLoadLog += "[ERROR] Missing fPluginLoad in " + std::string(findData.cFileName) + "\n";
            }
        } else {
            g_PluginLoadLog += "[ERROR] Failed to load " + std::string(findData.cFileName) + "\n";
        }
    } while (FindNextFileA(hFind, &findData));
    FindClose(hFind);
}
#elif defined(__ANDROID__)
#include <dlfcn.h>
#include <android/log.h>
#include <jni.h>
#include <SDL.h>
#include <string>
#include <dirent.h>

void DebugListDir(const std::string& path) {
    DIR* dir = opendir(path.c_str());
    if (!dir) {
        g_PluginLoadLog += "[ERROR] Cannot open dir: " + path + "\n";
        return;
    }

    g_PluginLoadLog += "[INFO] Listing dir: " + path + "\n";

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        g_PluginLoadLog += "  - " + std::string(entry->d_name) + "\n";
    }

    closedir(dir);
}

void LoadPlugins() {
    g_PluginLoadLog += "[INFO] Starting Android Plugin Loader\n";
    JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
    if (!env) {
        g_PluginLoadLog += "[FATAL] Failed to get JNIEnv\n";
        return;
    }

    jclass gameClass = env->FindClass("com/tele/u8emulator/Game");
    if (!gameClass) {
        g_PluginLoadLog += "[ERROR] FindClass failed for Game\n";
        return;
    }
    if (gameClass) {
        g_PluginLoadLog += "[INFO] Calling Java checkAndExtractPluginAssets()\n";
        jmethodID checkPluginMethod = env->GetStaticMethodID(gameClass, "checkAndExtractPluginAssets", "()V");
        if (checkPluginMethod) {
            env->CallStaticVoidMethod(gameClass, checkPluginMethod);
        }
    }

    const char* pluginsDir = getenv("CASIOEMU_PLUGINS_DIR");
    if (pluginsDir) {
        g_PluginLoadLog += "[INFO] Loading plugins from: " + std::string(pluginsDir) + "\n";
        
        std::string orderFilePath = std::string(pluginsDir) + "/load_order.txt";
        std::ifstream orderFile(orderFilePath);

        if (pluginsDir) {
            std::string dir = pluginsDir;
        
            DebugListDir("/data/user/0/com.tele.u8emulator/"); // 👈 check plugins_so
        
            // nếu muốn check luôn external
            //DebugListDir("/sdcard/Android/data/com.tele.u8emulator/files/plugins");
        
            g_PluginLoadLog += "[INFO] Loading plugins from: " + dir + "\n";
        }
        
        if (orderFile.is_open()) {
            std::string libName;
            while (std::getline(orderFile, libName)) {
                if (libName.empty()) continue;
                std::string fullPath = std::string(pluginsDir) + "/" + libName;
                g_PluginLoadLog += "[INFO] Attempting to load: " + libName + "\n";
                void* hModule = dlopen(fullPath.c_str(), RTLD_NOW | RTLD_GLOBAL);
                if (hModule) {
                    auto load = (PluginLoad)dlsym(hModule, "fPluginLoad");
                    if (load) {
                        load(g_pluginapi);
                        g_PluginLoadLog += "[SUCCESS] Loaded plugin: " + libName + "\n";
                    } else {
                        g_PluginLoadLog += "[INFO] Loaded dependency: " + libName + "\n";
                    }
                } else {
                    g_PluginLoadLog += "[ERROR] dlopen failed for " + libName + "\n";
                    g_PluginLoadLog += "Reason: " + std::string(dlerror()) + "\n";
                }
            }
        } else {
            g_PluginLoadLog += "[WARN] load_order.txt not found.\n";
        }
    }
}
#else
void LoadPlugins() {}
#endif