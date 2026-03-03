#include <imgui.h>
#include <android/log.h>

extern "C"
__attribute__((visibility("default")))
void PluginInit()
{
    __android_log_print(ANDROID_LOG_INFO, "PLUGIN",
        "DiddyDiddyDiddyDiddyDiddyDiddyDiddyDiddyDiddyDiddy"
        "DiddyDiddyDiddyDiddyDiddyDiddyDiddyDiddyDiddyDiddy"
        "DiddyDiddyDiddyDiddyDiddyDiddyDiddyDiddyDiddyDiddy");
}