#include <iostream>
#include <string>
#include <limits> 
#include <cmath>
#include <filesystem>

#include "FileProcessing.hpp"
#include "EMM.hpp"

using namespace std;
namespace fs = std::filesystem;

int main() {
    string inputPath, outputPath, gifPath;
    fs::path inputPathObj, outputPathObj, gifPathObj;
    int errorMethod;
    double threshold;
    int minBlockSize;
    double targetCompression;

    // === INPUTS ===

    while (true) {
        cout << "Alamat absolut gambar yang akan dikompresi: ";
        getline(cin, inputPath);
    
        inputPathObj = fs::path(inputPath);  
    
        if (!fs::exists(inputPathObj)) {
            cout << "File tidak ditemukan. Silakan masukkan ulang.\n";
        } else {
            string ext = inputPathObj.extension().string();
            if (ext != ".png" && ext != ".jpg" && ext != ".jpeg") {
                cout << "File bukan PNG atau JPG. Masukkan file yang valid.\n";
            } else {
                break;
            }
        }

        
    }

    FileProcessing::Image image = FileProcessing::loadImageAsDouble(inputPath);

    cout << "Pilih metode perhitungan error:\n";
    cout << "1. Variance\n";
    cout << "2. Mean Absolute Deviation (MAD)\n";
    cout << "3. Max Pixel Difference\n";
    cout << "4. Entropy\n";
    cout << "5. Masukkan nomor metode: ";
    cout << "6. Structural Similarity Index (SSIM)\n";

    while (!(cin >> errorMethod) || errorMethod < 1 || errorMethod > 6) {
        cout << "Metode tidak valid. Masukkan 1, 2, atau 3: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Ambang batas error (threshold): ";
    while (!(cin >> threshold) || threshold <= 0) {
        cout << "Threshold harus lebih dari 0: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Ukuran blok minimum (>= 1): ";
    while (!(cin >> minBlockSize) || minBlockSize < 1) {
        cout << "Ukuran blok harus >= 1: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Target persentase kompresi (0 = nonaktif, 1.0 = 100%): ";
    while (!(cin >> targetCompression) || targetCompression < 0.0 || targetCompression > 1.0) {
        cout << "Persentase kompresi harus di antara 0.0 hingga 1.0: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    cout << "Alamat output gambar hasil kompresi: ";
    getline(cin, outputPath);
    // TODO: Validate output path format or write permissions

    cout << "Alamat output GIF proses kompresi (optional, bonus): ";
    getline(cin, gifPath);
    // TODO: Support GIF output as bonus

    // === PROCESSING ===
    // TODO: Start timing execution
    // TODO: Read input image
    // TODO: Perform quadtree compression using selected error method and threshold
    // TODO: Adjust threshold dynamically if targetCompression > 0
    // TODO: Save compressed image and optionally generate GIF
    // TODO: End timing execution

    // === OUTPUTS ===
    cout << "\n=== HASIL EKSEKUSI ===" << endl;
    cout << "Waktu eksekusi: TODO detik" << endl;
    cout << "Ukuran gambar sebelum: TODO bytes" << endl;
    cout << "Ukuran gambar setelah: TODO bytes" << endl;
    cout << "Persentase kompresi: TODO %" << endl;
    cout << "Kedalaman pohon: TODO" << endl;
    cout << "Banyak simpul pada pohon: TODO" << endl;
    cout << "Gambar hasil kompresi disimpan di: " << outputPath << endl;
    if (!gifPath.empty()) {
        cout << "GIF proses kompresi disimpan di: " << gifPath << endl;
    }

    return 0;
}
