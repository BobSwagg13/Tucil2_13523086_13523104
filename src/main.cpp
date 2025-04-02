#include <iostream>
#include <string>
#include <limits> 
#include <cmath>
#include <filesystem>
#include <chrono>
#include <algorithm>

#include "FileProcessing.hpp"
#include "EMM.hpp"
#include "QuadTree.hpp"

using namespace std;
namespace fs = std::filesystem;

int main() {
    string inputPath, outputPath, gifPath, fileName;
    fs::path inputPathObj, gifPathObj;
    uintmax_t originalFileSize;
    int errorMethod;
    double threshold;
    int minBlockSize;
    double targetCompression;

    // === INPUTS ===

    while (true) {
        cout << "Alamat absolut gambar yang akan dikompresi: ";
        getline(cin, inputPath);
    
        inputPathObj = fs::path(inputPath);
 
        if (!inputPathObj.is_absolute()) {
            cout << "Path harus absolut. Masukkan path lengkap.\n";
            continue;
        }

        if (!fs::exists(inputPathObj)) {
            cout << "File tidak ditemukan. Silakan masukkan ulang.\n";
            continue;
        }

        string ext = inputPathObj.extension().string();
        for (char& c : ext) c = std::tolower(c);
 
        if (ext != ".png" && ext != ".jpg" && ext != ".jpeg") {
            cout << "File bukan PNG atau JPG. Masukkan file yang valid.\n";
            continue;
        }

        originalFileSize = fs::file_size(inputPath);
        break;
    }
    

    fs::path pathObj(inputPath);
    fileName = pathObj.filename().string();

    FileProcessing::Image image = FileProcessing::loadImageAsDouble(inputPath);

    cout << "Pilih metode perhitungan error:\n";
    cout << "1. Variance\n";
    cout << "2. Mean Absolute Deviation (MAD)\n";
    cout << "3. Max Pixel Difference\n";
    cout << "4. Entropy\n";
    cout << "5. Structural Similarity Index (SSIM)\n";
    cout << "Masukkan nomor metode: ";

    while (!(cin >> errorMethod) || errorMethod < 1 || errorMethod > 5) {
        cout << "Metode tidak valid. Masukkan hanya bisa dalam rentang 1 - 5: ";
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

    // === QUADTREE PROCESSING ===
    auto start = chrono::high_resolution_clock::now();
    auto compressedImage = image.data;

    QuadTree quadTree = QuadTree();
    QuadTree::Node* root = quadTree.buildTree(image.data, 0, 0, image.width, image.height);
    quadTree.divideTree(root, image.data, errorMethod, threshold, minBlockSize);
    quadTree.nodeToMatrix(root, compressedImage);

    string timestamp = FileProcessing::getCurrentDateTimeString();

    while (true) {
        std::cout << "[INPUT] Alamat absolut gambar hasil kompresi: ";
        std::getline(std::cin, outputPath);

        fs::path outputPathObj(outputPath);
        fs::path parentDir = outputPathObj.parent_path();
        std::string ext = outputPathObj.extension().string();

        for (char& c : ext) c = std::tolower(c);

        if (!outputPathObj.is_absolute()) {
            std::cout << "Path harus absolut. Masukkan path lengkap.\n";
            continue;
        }

        if (ext != ".jpg" && ext != ".jpeg" && ext != ".png") {
            std::cout << "Ekstensi file tidak valid! Gunakan .jpg, .jpeg, atau .png.\n";
            continue;
        }

        if (parentDir.empty() || fs::exists(parentDir)) {
            std::cout << "Direktori valid. File akan disimpan ke: " << outputPath << std::endl;
            break;
        } else {
            std::cout << "irektori tidak ditemukan: \"" << parentDir.string() << "\"\n";
            std::cout << "Silakan masukkan path yang valid.\n";
        }
    }
    

    //string compressedPath = "test/output/";
    //compressedPath += timestamp + "_" + fileName;
    FileProcessing::saveImageAsPNG(outputPath, compressedImage);
    uintmax_t compressedFileSize = fs::file_size(outputPath);

    int depth = quadTree.getDepth(root);
    int nodeCount = quadTree.countNode(root);

    quadTree.deleteTree(root);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    cout << "Alamat output GIF proses kompresi (optional, bonus): ";
    getline(cin, gifPath);
    // TODO: Support GIF output as bonus

    // === OUTPUTS ===
    double compressionRatio = 100 * (1.0 - (double)compressedFileSize / originalFileSize);

    cout << "\n=== HASIL EKSEKUSI ===" << endl;
    cout << "Waktu eksekusi: " << duration.count() << " detik" << endl;
    cout << "Ukuran gambar sebelum: "<< originalFileSize << endl;
    cout << "Ukuran gambar setelah: "<< compressedFileSize << endl;
    cout << "Persentase kompresi: "<< compressionRatio << "%" << endl;
    cout << "Kedalaman pohon: " << depth << endl;
    cout << "Banyak simpul pada pohon: " << nodeCount << endl;
    if (!gifPath.empty()) {
        cout << "GIF proses kompresi disimpan di: " << gifPath << endl;
    }

    return 0;
}
