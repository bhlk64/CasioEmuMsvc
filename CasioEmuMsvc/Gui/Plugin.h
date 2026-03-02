#pragma once
#include "Ui.hpp"
#include <vector>
#include <string>
#include <filesystem>

#ifdef __ANDROID__
using RenderFunc = void(*)();

extern std::vector<RenderFunc> active_plugin_renders;
void LoadEnabledPlugins();
void LoadPluginConfig();
#endif

class PluginViewer : public UIWindow
{
public:
    PluginViewer() : UIWindow("Plugin Manager")
    {
#ifdef __ANDROID__
        LoadPluginConfig();
        RefreshPlugins();
#endif
    }

    void RenderCore() override;
    
    std::vector<std::string> plugins;
    void SavePluginConfig();
    void RefreshPlugins();
};