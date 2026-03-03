#include "Plugin.h"
#include "imgui/imgui.h"
#include "SysDialog.h"
#include <algorithm>
#include <SDL.h>
#include <fstream>
#include <unordered_map>
#include <dlfcn.h>
#include <jni.h>
#include <SDL_system.h>
#include <android/log.h>
#include <vector>

#ifdef __ANDROID__

static std::unordered_map<std::string, bool> plugin_loaded;
static const std::filesystem::path plugin_config_path = "./config/plugins.cfg";
static bool need_restart = false;
static std::unordered_map<std::string, void*> plugin_handles;
static std::unordered_map<std::string, std::string> plugin_errors;
std::vector<RenderFunc> active_plugin_renders;

void PluginViewer::SavePluginConfig()
{
    std::filesystem::create_directories("./plugins");

    std::ofstream ofs(plugin_config_path);
    if (!ofs)
        return;

    for (const auto& [name, enabled] : plugin_loaded)
    {
        ofs << name << "=" << (enabled ? 1 : 0) << "\n";
    }
}

void LoadPluginConfig()
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

    for (auto& [name, handle] : plugin_handles) {
        if (handle) dlclose(handle);
    }
    
    plugin_handles.clear();
    plugin_errors.clear();
    active_plugin_renders.clear(); // Quan trọng: Reset danh sách vẽ
    
    std::filesystem::remove_all(
        std::filesystem::path(SDL_AndroidGetInternalStoragePath()) / "plugins"
    );

    LoadPluginConfig();
    
    std::filesystem::path src_dir = "./plugins";
    std::filesystem::path cache_dir = std::filesystem::path(SDL_AndroidGetInternalStoragePath()) / "plugins";
    std::filesystem::create_directories(cache_dir);

    for (const auto& [name, enabled] : plugin_loaded)
    {
        if (!enabled) continue;

        std::filesystem::path src = src_dir / name;
        std::filesystem::path dst = cache_dir / name;

        try {
            if (!std::filesystem::exists(src)) {
                plugin_errors[name] = "File not found";
                __android_log_print(ANDROID_LOG_ERROR, "PluginLoader", "Missing: %s", src.c_str());
                continue;
            }
            
            std::filesystem::copy_file(src, dst, std::filesystem::copy_options::overwrite_existing);

            // Load .so
            void* handle = dlopen(dst.string().c_str(), RTLD_NOW);

            //auto init   = (void(*)()) dlsym(handle, "PluginInit");
            //auto render = (void(*)()) dlsym(handle, "PluginRender");
            if (handle) {
                plugin_handles[name] = handle;
                __android_log_print(ANDROID_LOG_INFO, "PluginLoader", "Successfully loaded: %s", name.c_str());

                // 1. Gửi ImGui Context (Phải có tham số ImGuiContext*)
                using InitWithContext = void(*)(ImGuiContext*);
                InitWithContext init = (InitWithContext)dlsym(handle, "PluginInit");
                if (init) {
                    init(ImGui::GetCurrentContext());
                    __android_log_print(ANDROID_LOG_INFO, "PluginLoader", "Context shared for %s", name.c_str());
                }

                // 2. Lấy hàm vẽ và lưu vào vector
                RenderFunc draw = (RenderFunc)dlsym(handle, "PluginRender");
                if (draw) {
                    active_plugin_renders.push_back(draw);
                    __android_log_print(ANDROID_LOG_INFO, "PluginLoader", "Render function registered for %s", name.c_str());
                }
            } else {
                const char* err = dlerror();
                plugin_errors[name] = (err ? err : "dlopen failed");
                __android_log_print(ANDROID_LOG_ERROR, "PluginLoader", "Error: %s", plugin_errors[name].c_str());
            }
        } catch (const std::exception& e) {
            plugin_errors[name] = e.what();
            __android_log_print(ANDROID_LOG_ERROR, "PluginLoader", "Exception: %s", e.what());
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
    
        void* handle = nullptr;
        auto hit = plugin_handles.find(name);
        if (hit != plugin_handles.end())
            handle = hit->second;
    
        bool enabled = plugin_loaded[name];
    
        // ===== CHECKBOX LOAD =====
        if (ImGui::Checkbox("##enabled", &enabled))
        {
            plugin_loaded[name] = enabled;
            SavePluginConfig();
            need_restart = true; // vẫn cần restart do dlopen lifecycle
        }
    
        ImGui::SameLine();
        ImGui::Text("%s", name.c_str());
    
        ImGui::SameLine(300);
    
        // ===== STATUS TEXT =====
        if (handle)
            ImGui::TextColored(ImVec4(0,1,0,1), "Loaded");
        else if (enabled)
            ImGui::TextColored(ImVec4(1,0,0,1), "Error");
        else
            ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1), "Disabled");
    
        // ===== ERROR BUTTON (thay vì spam text) =====
        if (!handle && enabled)
        {
            auto err = plugin_errors.find(name);
            if (err != plugin_errors.end())
            {
                ImGui::SameLine();
                if (ImGui::Button("Details"))
                    ImGui::OpenPopup("err_popup");
            }
        }
    
        // ===== DELETE =====
        ImGui::SameLine();
        if (ImGui::Button("Delete"))
        {
            std::filesystem::remove("./plugins/" + name);
            plugin_loaded.erase(name);
            need_restart = true;
            RefreshPlugins();
            SavePluginConfig();
            ImGui::PopID();
            break;
        }
    
        // ===== ERROR POPUP =====
        if (ImGui::BeginPopupModal("err_popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            auto err = plugin_errors.find(name);
            if (err != plugin_errors.end())
            {
                static std::vector<char> buffer;
    
                buffer.assign(err->second.begin(), err->second.end());
                buffer.push_back('\0');
    
                ImGui::InputTextMultiline(
                    "##errtext",
                    buffer.data(),
                    buffer.size(),
                    ImVec2(500, 300),
                    ImGuiInputTextFlags_ReadOnly
                );
    
                if (ImGui::Button("Copy"))
                    ImGui::SetClipboardText(buffer.data());
            }
    
            if (ImGui::Button("Close"))
                ImGui::CloseCurrentPopup();
    
            ImGui::EndPopup();
        }
    
        ImGui::Separator();
        ImGui::PopID();
    }
}
#else
void PluginViewer::RenderCore()
{
    ImGui::Text("This features only available with Android");
}
#endif