import UIKit

@objc
public class SystemDialogs: NSObject {
    
    @objc public static func showNativeCrash(message: String) {
        print("SystemDialogs: Native crash: \(message)")
        
        DispatchQueue.main.async {
            guard let windowScene = UIApplication.shared.connectedScenes.first as? UIWindowScene,
                  let rootVC = windowScene.windows.first?.rootViewController else { return }
            
            let alert = UIAlertController(
                title: "Crash Detected",
                message: message,
                preferredStyle: .alert
            )
            
            alert.addAction(UIAlertAction(title: "Copy", style: .default, handler: { _ in
                UIPasteboard.general.string = message
                // Show a quick toast/alert to indicate copy success
                let toast = UIAlertController(title: nil, message: "Copied to clipboard", preferredStyle: .alert)
                rootVC.present(toast, animated: true)
                DispatchQueue.main.asyncAfter(deadline: .now() + 1.5) {
                    toast.dismiss(animated: true) {
                        exit(0)
                    }
                }
            }))
            
            alert.addAction(UIAlertAction(title: "Close", style: .cancel, handler: { _ in
                exit(0)
            }))
            
            rootVC.present(alert, animated: true, completion: nil)
        }
    }
}

// MARK: - C-Bridge for C++ Code

@_cdecl("onNativeCrash")
public func onNativeCrash(message: UnsafePointer<CChar>?) {
    guard let messagePtr = message else { return }
    let msgStr = String(cString: messagePtr)
    SystemDialogs.showNativeCrash(message: msgStr)
}
