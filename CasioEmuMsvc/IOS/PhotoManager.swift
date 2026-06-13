import UIKit
import Photos

@objc
public class PhotoManager: NSObject {
    
    @objc public static func saveImageToPhotoLibrary(bufferPtr: UnsafePointer<UInt8>, width: Int, height: Int, pitch: Int, filename: String) -> Bool {
        // Create CGContext from the raw ABGR/RGBA buffer (SDL standard is usually ABGR8888 or ARGB8888)
        // Assuming 32-bit RGBA for this example.
        let colorSpace = CGColorSpaceCreateDeviceRGB()
        let bitmapInfo = CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedLast.rawValue)
        
        guard let context = CGContext(data: UnsafeMutableRawPointer(mutating: bufferPtr),
                                      width: width,
                                      height: height,
                                      bitsPerComponent: 8,
                                      bytesPerRow: pitch,
                                      space: colorSpace,
                                      bitmapInfo: bitmapInfo.rawValue),
              let cgImage = context.makeImage() else {
            print("PhotoManager: Failed to create CGImage from buffer")
            return false
        }
        
        let image = UIImage(cgImage: cgImage)
        
        var success = false
        let semaphore = DispatchSemaphore(value: 0)
        
        PHPhotoLibrary.requestAuthorization { status in
            if status == .authorized || status == .limited {
                PHPhotoLibrary.shared().performChanges({
                    let request = PHAssetCreationRequest.forAsset()
                    request.addResource(with: .photo, data: image.pngData()!, options: nil)
                    // Unfortunately, setting a specific filename via PHAssetCreationRequest is tricky on iOS,
                    // but it will be saved as a PNG in the user's library.
                }) { saved, error in
                    if saved {
                        print("PhotoManager: Screenshot saved successfully")
                        DispatchQueue.main.async {
                            // Show toast
                            if let windowScene = UIApplication.shared.connectedScenes.first as? UIWindowScene,
                               let rootVC = windowScene.windows.first?.rootViewController {
                                let toast = UIAlertController(title: nil, message: "Screenshot saved to Photos", preferredStyle: .alert)
                                rootVC.present(toast, animated: true)
                                DispatchQueue.main.asyncAfter(deadline: .now() + 1.5) {
                                    toast.dismiss(animated: true)
                                }
                            }
                        }
                        success = true
                    } else if let error = error {
                        print("PhotoManager: Error saving photo: \(error)")
                    }
                    semaphore.signal()
                }
            } else {
                print("PhotoManager: Photo library access denied")
                semaphore.signal()
            }
        }
        
        _ = semaphore.wait(timeout: .now() + 10.0)
        return success
    }
}

// MARK: - C-Bridge for C++ Code

@_cdecl("saveImageToMediaStore")
public func saveImageToMediaStore(bufferPtr: UnsafePointer<UInt8>, width: Int, height: Int, pitch: Int, filenamePtr: UnsafePointer<CChar>?) -> Bool {
    let filename = filenamePtr != nil ? String(cString: filenamePtr!) : "screenshot.png"
    return PhotoManager.saveImageToPhotoLibrary(bufferPtr: bufferPtr, width: width, height: height, pitch: pitch, filename: filename)
}
