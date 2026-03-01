#include "Plugin.h"
#include "imgui/imgui.h"
#include "SysDialog.h"
#include <algorithm>
#include <unordered_map>
#include <SDL.h>

void PluginViewer::RefreshPlugins()
{
    plugins.clear();

    std::filesystem::path plugin_dir = "./plugins";

    if (!std::filesystem::exists(plugin_dir))
        return;

    for (auto& entry : std::filesystem::directory_iterator(plugin_dir))
    {
        if (entry.is_regular_file())
        {
            plugins.push_back(entry.path().filename().string());
        }
    }
}

void PluginViewer::RenderCore()
{
    static bool need_restart = false;
    static std::unordered_map<std::string, bool> plugin_loaded;

    if (ImGui::Button("Import Plugin"))
    {
        SystemDialogs::OpenFileDialog(
            [&](std::filesystem::path f)
            {
                try
                {
                    std::string ext = f.extension().string();
                    std::transform(ext.begin(), ext.end(),
                                   ext.begin(), ::tolower);

#ifdef _WIN32
                    bool valid = (ext == ".dll");
#else
                    bool valid = (ext == ".so");
#endif

                    if (!valid)
                        return;

                    std::filesystem::path plugin_dir = "./plugins";
                    std::filesystem::create_directories(plugin_dir);

                    std::filesystem::path dest =
                        plugin_dir / f.filename();

                    std::filesystem::copy_file(
                        f,
                        dest,
                        std::filesystem::copy_options::overwrite_existing
                    );

                    need_restart = true;  // 👈 báo cần restart
                    RefreshPlugins();
                }
                catch (...) {}
            });
    }

    if (need_restart)
    {
        ImGui::TextColored(ImVec4(1,1,0,1),
            "Restart App to reload Plugin");

        if (ImGui::Button("Exit"))
        {
            SDL_Quit();
            exit(0); // 👈 simple hard exit
        }
    }

    ImGui::Separator();

    for (size_t i = 0; i < plugins.size(); ++i)
    {
        ImGui::PushID((int)i);

        ImGui::Text("%s", plugins[i].c_str());
        ImGui::SameLine();

        // 👇 Checkbox Load/Unload
        bool& loaded = plugin_loaded[plugins[i]];
        if (ImGui::Checkbox("Load", &loaded))
        {
            need_restart = true;  // đổi trạng thái cũng cần restart
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete"))
        {
            std::filesystem::path full =
                std::filesystem::path("./plugins") / plugins[i];

            std::filesystem::remove(full);

            need_restart = true;
            RefreshPlugins();
            ImGui::PopID();
            break;
        }

        ImGui::PopID();
    }
}