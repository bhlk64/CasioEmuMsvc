#pragma once

#include <vector>
#include <string>
#include <filesystem>

class PluginViewer : public UIWindow
{
public:
    PluginViewer() : UIWindow("Plugin Manager")
    {
        RefreshPlugins();
    }

    void RenderCore() override;

private:
    std::vector<std::string> plugins;
    void RefreshPlugins();
};