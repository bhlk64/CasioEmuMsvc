#include "Plugin.h"
#include "imgui/imgui.h"
#include <algorithm>

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

                    RefreshPlugins();
                }
                catch (...) {}
            });
    }

    ImGui::Separator();

    // 👇 List plugin
    for (size_t i = 0; i < plugins.size(); ++i)
    {
        ImGui::PushID((int)i);

        ImGui::Text("%s", plugins[i].c_str());
        ImGui::SameLine();

        if (ImGui::Button("Delete"))
        {
            std::filesystem::path full =
                std::filesystem::path("./plugins") / plugins[i];

            std::filesystem::remove(full);

            RefreshPlugins();
            ImGui::PopID();
            break; // tránh invalid iterator
        }

        ImGui::PopID();
    }
}