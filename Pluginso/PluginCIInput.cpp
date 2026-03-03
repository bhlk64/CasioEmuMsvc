#include <imgui.h>
#include <android/log.h>

extern "C"
__attribute__((visibility("default")))
void PluginRender()
{
    __android_log_print(ANDROID_LOG_INFO, "PLUGIN", "Render called");

    ImGui::Begin("Plugin Alive");

    ImGui::Text("Hello from plugin 😈");
    ImGui::Text("If you see this, plugin OK.");

    ImGui::End();
}