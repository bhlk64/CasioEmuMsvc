#ifndef iOSNativeBridge_h
#define iOSNativeBridge_h

#ifdef __cplusplus
extern "C" {
#endif

void onFileSelected(const char* path, const unsigned char* data, int dataLength);
void onFileSaved(const char* path);
void onFolderSelected(const char* path);
void onFolderSaved(const char* path);
void onImportFailed();
void onExportFailed();

void onAppCreate();
void onAppResume();
void onAppPause();
void onAppBackground();
void onAppForeground();
void onAppTerminate();

float getSafeTop();

void nativeVibrate(long milliseconds);
void onNativeCrash(const char* message);

#ifdef __cplusplus
}
#endif

#endif