#pragma once
#include <filesystem>
#include <functional>
#include <string>

class SystemDialogs {
public:
    static void OpenFileDialog(std::function<void(std::filesystem::path)> callback);
    static void SaveFileDialog(std::string preferred_name, std::function<void(std::filesystem::path)> callback);
    static void OpenFolderDialog(std::function<void(std::filesystem::path)> callback);
    static void SaveFolderDialog(std::function<void(std::filesystem::path)> callback);
    /*#ifdef IOS
    static void onFileSelected(const char*, const unsigned char*, int);

    static void onFileSaved(const char*);
    static void onFolderSelected(const char*);
    static void onFolderSaved(const char*);
    static void onImportFailed();
    static void onExportFailed();
#endif*/

    static std::function<void(std::filesystem::path)> fileOpenCallback;
    static std::function<void(std::filesystem::path)> fileSaveCallback;
    static std::function<void(std::filesystem::path)> folderOpenCallback;
    static std::function<void(std::filesystem::path)> folderSaveCallback;
};