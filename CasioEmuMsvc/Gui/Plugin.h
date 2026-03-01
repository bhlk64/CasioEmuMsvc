#pragma once
#include "Ui.hpp"
#include <vector>
#include <string>
#include <filesystem>

#ifdef __ANDROID__
void LoadEnabledPlugins();
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

private:
#ifdef __ANDROID__
    std::vector<std::string> plugins;
    void LoadPluginConfig();
    void SavePluginConfig();
    void RefreshPlugins();
#endif
};