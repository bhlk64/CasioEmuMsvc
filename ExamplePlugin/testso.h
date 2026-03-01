#pragma once
#include "required/Ui.hpp"

#include "required/Ui.hpp"      // có UIWindow
#include "PluginApi.h"          // API của app
#include "imgui.h"

static PluginApi* api = nullptr;

class ExampleWindow : public UIWindow
{
public:
    ExampleWindow() : UIWindow("Example Plugin") {}

    void RenderCore() override
    {
        ImGui::Text("Hello from .so plugin 😎");

        static int counter = 0;

        if (ImGui::Button("Click me"))
            counter++;

        ImGui::SameLine();
        ImGui::Text("Count = %d", counter);
    }
};

extern "C"
void PluginInit(PluginApi* pApi)
{
    api = pApi;
    if (!api) return;

    if (!api->RegisterPlugin("example", "Example plugin", 1))
        return;

    api->AddWindow(new ExampleWindow());
}