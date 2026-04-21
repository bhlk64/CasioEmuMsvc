#include "FileExplorerWindow.hpp"
#include "imgui/imgui.h"
#include <fstream>
#include <sstream>

FileExplorerWindow::FileExplorerWindow(const std::string& startPath)
    : UIWindow("File Explorer"), currentPath(startPath) {}

void FileExplorerWindow::LoadFile(const fs::path& file) {
    previewContent.clear();

    std::ifstream f(file, std::ios::binary);
    if (!f.is_open()) {
        previewContent = "Cannot open file.";
        return;
    }

    std::ostringstream ss;
    ss << f.rdbuf();

    previewContent = ss.str();

    // limit tránh lag UI
    if (previewContent.size() > 10000)
        previewContent = previewContent.substr(0, 10000) + "\n...[TRUNCATED]";
}

void FileExplorerWindow::RenderCore() {
    ImGui::BeginChild("explorer");

    if (ImGui::Button("..")) {
        if (currentPath.has_parent_path())
            currentPath = currentPath.parent_path();
    }

    ImGui::SameLine();
    ImGui::Text("%s", currentPath.string().c_str());

    ImGui::Separator();

    for (auto& entry : fs::directory_iterator(currentPath)) {
        auto p = entry.path();
        std::string name = p.filename().string();

        if (entry.is_directory()) {
            if (ImGui::Selectable(("[DIR] " + name).c_str())) {
                currentPath = p;
            }
        } else {
            if (ImGui::Selectable(name.c_str())) {
                selectedFile = p.string();
                LoadFile(p);
                showPreview = true;
            }
        }
    }

    ImGui::EndChild();

    ImGui::Separator();

    ImGui::BeginChild("preview", ImVec2(0, 0), true);

    ImGui::Text("Selected:");
    ImGui::TextWrapped("%s", selectedFile.c_str());

    ImGui::Separator();

    if (showPreview) {
        ImGui::TextUnformatted(previewContent.c_str());
    } else {
        ImGui::Text("Click file to preview");
    }

    ImGui::EndChild();
}