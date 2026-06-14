import UIKit
import UniformTypeIdentifiers

@objc
public class FileManager: NSObject, UIDocumentPickerDelegate {
    
    public static let shared = FileManager()
    
    private var pendingExportData: Data?
    
    // Extract assets like roms.db and locales from main bundle to Documents
    @objc public func extractAssets() {
        let fileManager = Foundation.FileManager.default
        guard let documentsURL = fileManager.urls(for: .documentDirectory, in: .userDomainMask).first else { return }
        
        // Extract roms.db
        let romsDbDest = documentsURL.appendingPathComponent("roms.db")
        if !fileManager.fileExists(atPath: romsDbDest.path) {
            if let romsDbSource = Bundle.main.url(forResource: "roms", withExtension: "db") {
                do {
                    try fileManager.copyItem(at: romsDbSource, to: romsDbDest)
                    print("FileManager: Extracted roms.db")
                } catch {
                    print("FileManager: Failed to extract roms.db: \(error)")
                }
            }
        }
        
        // Extract locales folder
        let localesDest = documentsURL.appendingPathComponent("locales")
        if !fileManager.fileExists(atPath: localesDest.path) {
            do {
                try fileManager.createDirectory(at: localesDest, withIntermediateDirectories: true)
                if let localesURLs = Bundle.main.urls(forResourcesWithExtension: nil, subdirectory: "locales") {
                    for url in localesURLs {
                        let destURL = localesDest.appendingPathComponent(url.lastPathComponent)
                        try fileManager.copyItem(at: url, to: destURL)
                    }
                }
                print("FileManager: Extracted locales")
            } catch {
                print("FileManager: Failed to extract locales: \(error)")
            }
        }
    }
    
    // UI for Exporting Data
    public func exportData(data: Data, suggestedFilename: String) {
        DispatchQueue.main.async {
            self.pendingExportData = data
            
            // Write data to a temporary file to pass to UIDocumentPicker
            let tempDir = Foundation.FileManager.default.temporaryDirectory
            let tempFile = tempDir.appendingPathComponent(suggestedFilename)
            
            do {
                try data.write(to: tempFile)
                let documentPicker = UIDocumentPickerViewController(forExporting: [tempFile])
                documentPicker.delegate = self
                
                if let windowScene = UIApplication.shared.connectedScenes.first as? UIWindowScene,
                   let rootVC = windowScene.windows.first?.rootViewController {
                    rootVC.present(documentPicker, animated: true, completion: nil)
                }
            } catch {
                print("FileManager: Failed to write temp file for export: \(error)")
                c_onExportFailed()
            }
        }
    }
    
    // MARK: - UIDocumentPickerDelegate
    
    public func documentPicker(_ controller: UIDocumentPickerViewController, didPickDocumentsAt urls: [URL]) {
        guard let url = urls.first else {
            c_onExportFailed()
            return
        }
        
        print("FileManager: Exported to \(url.path)")
        url.withUnsafeFileSystemRepresentation { pathPtr in
            if let pathPtr = pathPtr {
                c_onFileSaved(pathPtr)
            }
        }
    }
    
    public func documentPickerWasCancelled(_ controller: UIDocumentPickerViewController) {
        print("FileManager: Export cancelled")
        c_onExportFailed()
    }
}

// MARK: - C-Bridge for C++ Code

@_cdecl("exportData")
public func exportData(dataPtr: UnsafePointer<UInt8>, length: Int, uriPtr: UnsafePointer<CChar>?) {
    let data = Data(bytes: dataPtr, count: length)
    let filename = uriPtr != nil ? String(cString: uriPtr!) : "export.package"
    FileManager.shared.exportData(data: data, suggestedFilename: filename)
}

// Extern definitions for C++ to provide
@_silgen_name("onFileSaved")
func c_onFileSaved(_ path: UnsafePointer<CChar>)

@_silgen_name("onExportFailed")
func c_onExportFailed()
