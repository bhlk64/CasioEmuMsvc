#include "PluginLogWindow.hpp"
#include "Plugin/PluginMan.h"
#include "imgui/imgui.h"
#include "../Ext/SysDialog.h"
#include <sstream>
#include <filesystem>
#include <cstdlib>
#include <fstream>

PluginLogWindow::PluginLogWindow() : UIWindow("Plugin Manager") {
    inital_size = ImVec2(700, 450);
    open = false;
}

void PluginLogWindow::RenderCore() {
    if (ImGui::BeginTabBar("PluginTabs")) {
        if (ImGui::BeginTabItem("Loaded Plugins")) {
#if __ANDROID__
            if (ImGui::Button("Import Plugin")) {
                SystemDialogs::OpenFileDialog([&](std::filesystem::path f) {
                    try {
                        std::ifstream ifs{f, std::ios::binary};
                        if (!ifs) {
                            throw std::runtime_error("Cannot open selected file.");
                        }
                
                        auto name = f.filename().string();
                
                        if (!name.ends_with(".so")) {
                            g_PluginLoadLog += "[ERROR] Not a .so file: " + name + "\n";
                            return;
                        }
                
                        std::filesystem::path dstDir = "/data/user/0/com.tele.u8emulator/cache/plugins_so";
                        std::filesystem::create_directories(dstDir);
                
                        std::filesystem::path dst = dstDir / name;
                
                        std::filesystem::copy_file(
                            f,
                            dst,
                            std::filesystem::copy_options::overwrite_existing
                        );
                        g_PluginLoadLog += "[SUCCESS] Imported: " + name + "\n";
                        g_PluginLoadLog += "         -> " + dst.string() + "\n";
                    }
                    catch (const std::exception& e) {
                        g_PluginLoadLog += "[ERROR] Import failed: ";
                        g_PluginLoadLog += e.what();
                        g_PluginLoadLog += "\n";
                    }
                });
            ImGui::Separator();
            }
#endif
            if (ImGui::BeginTable("PluginsTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
                ImGui::TableSetupColumn("ID");
                ImGui::TableSetupColumn("Name");
                ImGui::TableSetupColumn("Version");
                ImGui::TableSetupColumn("Author");
                ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();
                
                for (const auto& p : g_loadedPlugins) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0); ImGui::TextUnformatted(p.id.c_str());
                    ImGui::TableSetColumnIndex(1); ImGui::TextUnformatted(p.name.c_str());
                    ImGui::TableSetColumnIndex(2); ImGui::TextUnformatted(p.version.c_str());
                    ImGui::TableSetColumnIndex(3); ImGui::TextUnformatted(p.author.c_str());
                    ImGui::TableSetColumnIndex(4); ImGui::TextWrapped("%s", p.desc.c_str());
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Loader Log")) {
            if (ImGui::Button("Copy to Clipboard")) {
                ImGui::SetClipboardText(g_PluginLoadLog.c_str());
            }
            ImGui::SameLine();
            if (ImGui::Button("Clear Log")) {
                g_PluginLoadLog.clear();
            }
            
            ImGui::Separator();
            
            ImGui::BeginChild("LogScrollingRegion", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
            
            std::istringstream stream(g_PluginLoadLog);
            std::string line;
            while (std::getline(stream, line)) {
                if (line.find("[ERROR]") != std::string::npos || line.find("[FATAL]") != std::string::npos) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
                } else if (line.find("[WARN]") != std::string::npos) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.2f, 1.0f));
                } else if (line.find("[SUCCESS]") != std::string::npos) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 1.0f, 0.4f, 1.0f));
                } else {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_Text]);
                }
                
                ImGui::TextUnformatted(line.c_str());
                ImGui::PopStyleColor();
            }
            
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}