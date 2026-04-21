#pragma once
#include "Ui.hpp"
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class FileExplorerWindow : public UIWindow {
private:
    fs::path currentPath;
    std::string selectedFile;

    bool showPreview = false;
    std::string previewContent;

    void LoadFile(const fs::path& file);

public:
    FileExplorerWindow(const std::string& startPath);

    void RenderCore() override;
};