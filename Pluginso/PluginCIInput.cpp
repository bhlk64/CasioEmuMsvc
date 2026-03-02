#include <imgui.h>
#include <android/log.h>

// Bắt buộc phải có extern "C" để dlsym tìm thấy tên hàm chính xác
extern "C" {

    // 1. Hàm khở tạo: Nhận context từ App chính
    void PluginInit(ImGuiContext* ctx) {
        if (ctx) {
            ImGui::SetCurrentContext(ctx);
            __android_log_print(ANDROID_LOG_INFO, "CasioTestPlugin", "Context shared successfully!");
        }
    }

    // 2. Hàm vẽ: Sẽ được gọi mỗi frame trong gui_loop()
    void PluginRender() {
        // Tạo một cửa sổ ImGui mới
        ImGui::Begin("Casio Debug Plugin");
        
        ImGui::Text("System Status: Online");
        ImGui::Separator();
        
        static float val = 0.5f;
        ImGui::SliderFloat("CPU Speed Overlay", &val, 0.0f, 1.0f);
        
        if (ImGui::Button("Log to Logcat")) {
            __android_log_print(ANDROID_LOG_INFO, "CasioTestPlugin", "Hello from Plugin UI!");
        }

        ImGui::End();

        // Bạn cũng có thể mở Demo Window để test độ ổn định
        // static bool show_demo = true;
        // ImGui::ShowDemoWindow(&show_demo);
    }
}
