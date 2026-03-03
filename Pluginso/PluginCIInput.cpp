#include "imgui.h"
#include <android/log.h>

extern "C" {

void PluginInit(ImGuiContext* ctx)
{
    ImGui::SetCurrentContext(ctx);
    __android_log_print(ANDROID_LOG_ERROR, "PLUGIN", "INIT OK");
}

void PluginRender()
{
    __android_log_print(ANDROID_LOG_INFO, "PLUGIN", "RENDER OK");

    ImGui::Begin("Test Plugin");
    ImGui::Text("Hello from plugin");
    ImGui::End();
}

}