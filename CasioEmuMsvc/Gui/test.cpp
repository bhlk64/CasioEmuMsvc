#include "test.h"
#include "imgui/imgui.h"

void TestViewer::RenderCore()
{
    ImGui::Text("Hello plugin 😎");

    if (ImGui::Button("Click")) {
        ImGui::Text("Bruh");
    }
}