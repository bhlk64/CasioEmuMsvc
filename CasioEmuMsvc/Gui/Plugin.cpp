#include "Plugin.h"
#include "imgui/imgui.h"
#include "SysDialog.h"
#include <algorithm>
#include <SDL.h>
#include <fstream>
#include <unordered_map>

static std::unordered_map<std::string, bool> plugin_loaded;
static const std::filesystem::path plugin_config_path = "./config/plugins.cfg";
static bool need_restart = false;

void PluginViewer::SavePluginConfig()
{
    std::filesystem::create_directories("./config");

    std::ofstream ofs(plugin_config_path);
    if (!ofs)
        return;

    for (const auto& [name, enabled] : plugin_loaded)
    {
        ofs << name << "=" << (enabled ? 1 : 0) << "\n";
    }
}

void PluginViewer::LoadPluginConfig()
{
    plugin_loaded.clear();

    std::ifstream ifs(plugin_config_path);
    if (!ifs) return;

    std::string line;
    while (std::getline(ifs, line))
    {
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string name = line.substr(0, pos);
        bool enabled = (line.substr(pos + 1) == "1");

        plugin_loaded[name] = enabled;
    }
}

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
            std::string filename = entry.path().filename().string();
            plugins.push_back(filename);

            // nếu plugin mới chưa có trong config
            if (plugin_loaded.find(filename) == plugin_loaded.end())
            {
                plugin_loaded[filename] = false;
            }
        }
    }
}

void PluginViewer::RenderCore()
{
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

                    bool valid = (ext == ".so");

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
                    SavePluginConfig();
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
            SavePluginConfig();   // 👈 thêm dòng này
            need_restart = true;
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete"))
        {
            std::filesystem::path full =
                std::filesystem::path("./plugins") / plugins[i];

            std::filesystem::remove(full);

            need_restart = true;
            RefreshPlugins();
            SavePluginConfig();
            ImGui::PopID();
            break;
        }

        ImGui::PopID();
    }
}