#include "Plugin.h"
#include "imgui/imgui.h"
#include "SysDialog.h"
#include <algorithm>
#include <SDL.h>
#include <fstream>
#include <unordered_map>
#include <dlfcn.h>

#ifdef __ANDROID__
static std::unordered_map<std::string, bool> plugin_loaded;
static const std::filesystem::path plugin_config_path = "./config/plugins.cfg";
static bool need_restart = false;
static std::unordered_map<std::string, void*> plugin_handles;
static std::unordered_map<std::string, std::string> plugin_errors;

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
    std::filesystem::path src_dir = "./plugins";
    while (std::getline(ifs, line))
    {
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string name = line.substr(0, pos);
        bool enabled = (line.substr(pos + 1) == "1");

        plugin_loaded[name] = enabled;
    }
    for (auto it = plugin_loaded.begin(); it != plugin_loaded.end(); )
    {
        if (!std::filesystem::exists(src_dir / it->first))
            it = plugin_loaded.erase(it);
        else
            ++it;
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
        if (entry.is_regular_file() && entry.path().extension() == ".so")
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

void LoadEnabledPlugins()
{
    for (auto& [name, handle] : plugin_handles)
        {
            if (handle)
                dlclose(handle);
        }

    plugin_handles.clear();
    plugin_errors.clear();
    PluginViewer viewer;
    viewer.LoadPluginConfig();
    std::filesystem::path src_dir = "./plugins";
    std::filesystem::path cache_dir = "./cache";

    std::filesystem::create_directories(cache_dir);

    for (const auto& [name, enabled] : plugin_loaded)
    {
        if (!enabled)
            continue;

        std::filesystem::path src = src_dir / name;
        std::filesystem::path dst = cache_dir / name;

        try
        {
            // copy vào internal
            if (!std::filesystem::exists(src))
            {
                plugin_errors[name] = "File not found";
                continue;
            }
            std::filesystem::copy_file(
                src,
                dst,
                std::filesystem::copy_options::overwrite_existing
            );
            plugin_errors.erase(name);

            // load từ cache
            void* handle = dlopen(dst.c_str(), RTLD_NOW);

            if (handle)
            {
                plugin_handles[name] = handle;
                
                dlerror(); // clear old error
            
                using InitFunc = void(*)();
                InitFunc init = (InitFunc)dlsym(handle, "PluginInit");
            
                if (init)
                    init();
            }
            else
            {
                plugin_handles[name] = nullptr;
                plugin_errors[name] = dlerror();
            }
        }
        catch (...)
        {
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
    
        const std::string& name = plugins[i];
    
        // lấy handle an toàn
        void* handle = nullptr;
        auto it = plugin_handles.find(name);
        if (it != plugin_handles.end())
            handle = it->second;
    
        bool enabled = plugin_loaded[name];
    
        // ===== STATUS =====
        if (handle)
        {
            ImGui::TextColored(ImVec4(0,1,0,1), "[Loaded]");
        }
        else if (enabled)
        {
            ImGui::TextColored(ImVec4(1,0,0,1), "[Error]");
        }
        else
        {
            ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1), "[Disabled]");
        }
    
        ImGui::SameLine();
        ImGui::Text("%s", name.c_str());
    
        // ===== ERROR MESSAGE INLINE =====
        if (!handle && enabled)
        {
            auto err_it = plugin_errors.find(name);
            if (err_it != plugin_errors.end())
            {
                ImGui::SameLine();
                ImGui::TextColored(
                    ImVec4(1,0.5f,0.5f,1),
                    "(%s)",
                    err_it->second.c_str()
                );
            }
        }
    
        ImGui::SameLine();
    
        if (ImGui::Button("Delete"))
        {
            std::filesystem::path full =
                std::filesystem::path("./plugins") / name;
    
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
#else
void PluginViewer::RenderCore()
{
    ImGui::Text("This features only available with Android");
}
#endif