#ifndef FPS_MANAGER_H
#define FPS_MANAGER_H

#include <chrono>
#include <thread>

class FPSManager {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime;
    long long targetFrameTimeMs;
    
    // Biến lưu trữ trạng thái pixel của khung hình trước để lọc trùng
    unsigned int lastFrameHash;

public:
    FPSManager() {
        targetFrameTimeMs = 33; // Khóa cứng 30 FPS
        lastFrameTime = std::chrono::high_resolution_clock::now();
        lastFrameHash = 0;
    }

    // Hàm lọc thông minh: Trả về true nếu khung hình có sự thay đổi, false nếu bị trùng
    bool filterFrame(const void* pixelData, size_t bufferSize) {
        if (!pixelData || bufferSize == 0) return true;

        // Thuật toán băm nhanh (MurmurHash/FNV đơn giản) để kiểm tra thay đổi màn hình
        unsigned int currentHash = 2166136261U;
        const unsigned char* bytes = static_cast<const unsigned char*>(pixelData);
        
        // Lấy mẫu nhanh một số điểm ảnh đại diện để tiết kiệm CPU thay vì quét toàn bộ
        size_t step = bufferSize / 100; 
        if (step == 0) step = 1;
        for (size_t i = 0; i < bufferSize; i += step) {
            currentHash ^= bytes[i];
            currentHash *= 16777619U;
        }

        // Nếu khung hình trùng lặp hoàn toàn với khung hình trước
        if (currentHash == lastFrameHash) {
            return false; // Lọc bỏ, không cho Render lại
        }

        lastFrameHash = currentHash;
        return true; // Khung hình mới, cho phép vẽ
    }

    // Ép CPU ngủ sâu để hạ nhiệt
    void limit() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFrameTime).count();

        if (elapsedTime < targetFrameTimeMs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(targetFrameTimeMs - elapsedTime));
        }

        lastFrameTime = std::chrono::high_resolution_clock::now();
    }
};

#endif // FPS_MANAGER_H
