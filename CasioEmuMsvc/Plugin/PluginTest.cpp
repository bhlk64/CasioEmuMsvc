#include "PluginApi.h"
#include "imgui/imgui.h"

class MyWindow : public IWindow {
public:
    void RenderCore() override {
        ImGui::Text("Hello from plugin 😎");
        if (ImGui::Button("Click me")) {
            // nothing, chỉ cho vui
        }
    }
};

static MyWindow g_window;

extern "C" void PluginLoad(PluginApi* api)
{
    api->RegisterPlugin("MyPlugin", "Test Native Plugin", 1);
    api->AddWindow(&g_window);
}