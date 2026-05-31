#ifndef FPS_MANAGER_H
#define FPS_MANAGER_H

#include <chrono>
#include <thread>

class FPSManager {
private:
    std::chrono::time_point<std::chrono::steady_clock> lastFrameTime;
    unsigned int targetFrameTimeMs;

public:
    // Khởi tạo mặc định khóa ở 30 FPS (~33ms mỗi khung hình)
    FPSManager(unsigned int fps = 30) {
        targetFrameTimeMs = 1000 / fps;
        lastFrameTime = std::chrono::steady_clock::now();
    }

    // Hàm này gọi ở cuối mỗi vòng lặp để kiểm soát tốc độ khung hình
    void limit() {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFrameTime).count();

        if (elapsedTime < targetFrameTimeMs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(targetFrameTimeMs - elapsedTime));
        }

        lastFrameTime = std::chrono::steady_clock::now();
    }
};

#endif // FPS_MANAGER_H
