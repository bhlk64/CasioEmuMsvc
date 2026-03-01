#include "Plugin.h"
#include "imgui/imgui.h"

void PluginViewer::RenderCore()
{
    ImGui::Text("Hello plugin 😎");

    if (ImGui::Button("Click")) {
        ImGui::Text("Bruh");
    }
}