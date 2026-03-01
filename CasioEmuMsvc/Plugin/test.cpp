#include "pch.h"
#include "PluginApi.h"

class SimpleWindow : public UIWindow
{
public:
    SimpleWindow() : UIWindow("Simple Window") {}

    void RenderCore() override
    {
        ImGui::Text("Test window nho xinh");

        if (ImGui::Button("Bam toi di"))
        {
            ImGui::OpenPopup("popup");
        }

        if (ImGui::BeginPopup("popup"))
        {
            ImGui::Text("Ban vua bam nut 😎");
            ImGui::EndPopup();
        }
    }
};

extern "C" __declspec(dllexport)
void fPluginLoad(PluginApi* api)
{
    if (!api) return;

    if (!api->RegisterPlugin("simple", "simple plugin", 1))
        return;

    api->AddWindow(new SimpleWindow());
}