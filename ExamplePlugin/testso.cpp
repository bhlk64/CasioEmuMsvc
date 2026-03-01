#include "PluginApi.h"
#include <vector>
#include <string>

static PluginApi* g_api = nullptr;

// ==============================
// Example Window
// ==============================
class ExampleWindow : public UIWindow
{
public:
    ExampleWindow() : UIWindow("Example Plugin")
    {
        inital_size = { 400, 300 };
    }

    void RenderCore() override
    {
        ImGui::Text("Hello from Example .so plugin 😎");
        ImGui::Separator();

        static int counter = 0;

        if (ImGui::Button("Click me"))
            counter++;

        ImGui::SameLine();
        ImGui::Text("Count = %d", counter);

        ImGui::Separator();

        if (ImGui::Button("Close window"))
            open = false;
    }
};

// ==============================
// Entry Point (IMPORTANT)
// ==============================
extern "C"
void PluginInit(PluginApi* api)
{
    if (!api) return;

    // check STL compatibility
    PLUGINASSERTSTL((*api));

    // register plugin
    if (!api->RegisterPlugin("example", "Example Plugin", 1))
        return;

    g_api = api;

    // add window to app
    api->AddWindow(new ExampleWindow());
}