import UIKit
import CoreHaptics

@objc
public class Game: UIResponder, UIApplicationDelegate {
    
    public static let shared = Game()
    
    private var backgroundTask: UIBackgroundTaskIdentifier = .invalid
    private var stopEmulationTimer: Timer?
    
    // Equivalent of Android's onPause() / Background timer
    @objc public func appDidEnterBackground() {
        print("Game: App entered background")
        // Start background task
        backgroundTask = UIApplication.shared.beginBackgroundTask {
            self.endBackgroundTask()
        }
        
        // Android stops emulation after 5 minutes (300,000 ms)
        stopEmulationTimer = Timer.scheduledTimer(withTimeInterval: 300.0, repeats: false) { [weak self] _ in
            self?.stopEmulation()
        }
    }
    
    // Equivalent of Android's onResume()
    @objc public func appWillEnterForeground() {
        print("Game: App entered foreground")
        stopEmulationTimer?.invalidate()
        stopEmulationTimer = nil
        endBackgroundTask()
    }
    
    private func endBackgroundTask() {
        if backgroundTask != .invalid {
            UIApplication.shared.endBackgroundTask(backgroundTask)
            backgroundTask = .invalid
        }
    }
    
    private func stopEmulation() {
        print("Game: Emulation stopped due to 5 mins in background")
        // In iOS, we shouldn't force kill the app via exit(0), but to mirror Android:
        exit(0)
    }
    
    @objc public func vibrate(milliseconds: Int) {
        // iOS doesn't support exact millisecond vibration easily without CoreHaptics engines,
        // so we use standard UIImpactFeedbackGenerator as a close approximation.
        DispatchQueue.main.async {
            let generator = UIImpactFeedbackGenerator(style: .medium)
            generator.prepare()
            generator.impactOccurred()
        }
    }
}

// MARK: - C-Bridge for C++ Code

@_cdecl("nativeVibrate")
public func nativeVibrate(milliseconds: Int64) {
    Game.shared.vibrate(milliseconds: Int(milliseconds))
}

@_cdecl("createModelShortcut")
public func createModelShortcut(modelPath: UnsafePointer<CChar>?, shortcutName: UnsafePointer<CChar>?, iconPath: UnsafePointer<CChar>?) {
    DispatchQueue.main.async {
        guard let windowScene = UIApplication.shared.connectedScenes.first as? UIWindowScene,
              let rootVC = windowScene.windows.first?.rootViewController else { return }
        
        let alert = UIAlertController(
            title: "Tạo Shortcut",
            message: "Trên iOS, bạn không thể tạo lối tắt ngoài màn hình chính trực tiếp từ ứng dụng. Vui lòng sử dụng tính năng 'Add to Home Screen' của Safari hoặc dùng ứng dụng Shortcuts của Apple.",
            preferredStyle: .alert
        )
        alert.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
        rootVC.present(alert, animated: true, completion: nil)
    }
}
