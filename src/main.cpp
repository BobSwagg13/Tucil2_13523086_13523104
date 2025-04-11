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
    string inputPath, outputPath, gifPath, fileName, fileType;
    fs::path inputPathObj, gifPathObj;
    uintmax_t originalFileSize;
    int errorMethod;
    double threshold;
    int minBlockSize;
    double targetCompression;
    bool targetCompressionEnabled = false;
    bool targetCompressionValid = false; 

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
        fileType = ext;
        for (char& c : ext) c = std::tolower(c);
        cout << "File type: " << fileType << endl;
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

    cout << "Target persentase kompresi 0 - 100%: ";
    while (!(cin >> targetCompression) || targetCompression < 0.0 || targetCompression > 100.0) {
        cout << "Persentase kompresi harus di antara 0 hingga 100: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (targetCompression > 0.0) {
        targetCompressionEnabled = true;
        cout << "Target persentase kompresi diaktifkan: " << targetCompression << "%" << endl;
    } else {
        cout << "Target persentase kompresi dinonaktifkan." << endl;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    // === QUADTREE PROCESSING ===
    
    auto start = chrono::high_resolution_clock::now();
    
    auto compressedImage = image.data;

    QuadTree quadTree;
    QuadTree::Node* root;
    double maxThreshold;
    if(errorMethod == 1) {
        maxThreshold = 16256.25; 
    } else if (errorMethod == 2) {
        maxThreshold = 127.5;
    } else if (errorMethod == 3) {
        maxThreshold = 255;
    } else if (errorMethod == 4) {
        maxThreshold = 8.0;
    } else if (errorMethod == 5) {
        maxThreshold = 1.0;
    }

    double low = 0.0, high = maxThreshold, tolerance = 0.01;
    int maxIterations = 50, iteration = 0;

    while (!targetCompressionValid && iteration < maxIterations) {
        compressedImage = image.data;
        if (targetCompressionEnabled) {
            threshold = (low + high) / 2.0;
        }

        quadTree = QuadTree();
        root = quadTree.buildTree(image.data, 0, 0, image.width, image.height);
        quadTree.divideTree(root, image.data, errorMethod, threshold, minBlockSize);
        quadTree.nodeToMatrix(root, compressedImage);

        if (!targetCompressionEnabled) {
            break;
        } else {
            fs::create_directories("targetCompressionTemp");

            outputPath = "targetCompressionTemp/" + fileName;
            FileProcessing::saveImageAsPNG(outputPath, compressedImage);
            uintmax_t compressedFileSize = fs::file_size(outputPath);
            fs::remove(outputPath);

            double compressionNew = 1.0 - static_cast<double>(compressedFileSize) / originalFileSize;

            if (abs((targetCompression / 100.0) - compressionNew) <= tolerance) {
                targetCompressionValid = true;
            } else {
                if (compressionNew < (targetCompression / 100.0)) {
                    if(errorMethod < 5){
                        low = threshold;
                    }
                    else{
                        high = threshold;
                    }
                } else {
                    if(errorMethod < 5){
                        high = threshold;
                    }
                    else{
                        low = threshold;
                    }
                }
            }
            
            cout << "Compressed file size: " << compressedFileSize << " bytes" << endl;
            cout << "Target compression: " << targetCompression << "%" << endl;
            cout << iteration << " Threshold: " << threshold<< ", Compression ratio: " << compressionNew << endl;
        }
        iteration++;
    }
    
    fs::create_directories("gifTemp");
    int counter = 0;
    QuadTree::bfsAveragePerLevelImage(root, image.data, [&counter, fileType](const std::vector<std::vector<std::vector<double>>>& img, int level) {
        if(fileType == ".png"){
            FileProcessing::saveImageAsPNG("gifTemp/level_" + std::to_string(level + 100) + ".png", img);
        }
        else if(fileType == ".jpg" || fileType == ".jpeg"){
            FileProcessing::saveImageAsPNG("gifTemp/level_" + std::to_string(level + 100) + ".jpg", img);
        }
        else{
            cout << "Ekstensi file tidak valid!" << endl;
        }
        counter += 1;
    });
    
    auto end = chrono::high_resolution_clock::now();

    while (true) {
        std::cout << "Alamat absolut gambar hasil kompresi: ";
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
            break;
        } else {
            std::cout << "Direktori tidak ditemukan: \"" << parentDir.string() << "\"\n";
            std::cout << "Silakan masukkan path yang valid.\n";
        }
    }
    
    while (true) {
        std::cout << "Alamat output GIF proses kompresi: ";
        std::getline(std::cin, gifPath);

        gifPath.erase(std::remove(gifPath.begin(), gifPath.end(), ' '), gifPath.end());

        fs::path outputPathObj(gifPath);
        fs::path parentDir = outputPathObj.parent_path();
        std::string ext = outputPathObj.extension().string();

        for (char& c : ext) c = std::tolower(c);

        if (!outputPathObj.is_absolute()) {
            std::cout << "Path harus absolut. Masukkan path lengkap.\n";
            continue;
        }

        if (ext != ".gif") {
            std::cout << "Ekstensi file tidak valid! Gunakan .gif.\n";
            continue;
        }

        if (parentDir.empty() || fs::exists(parentDir)) {
            break;
        } else {
            std::cout << "Direktori tidak ditemukan: \"" << parentDir.string() << "\"\n";
            std::cout << "Silakan masukkan path yang valid.\n";
        }
    }

    FileProcessing::saveImageAsPNG(outputPath, compressedImage);
    cout << "Saving GIF to " << gifPath << endl;

    string command = "convert -delay 50 -loop 0 ";

    for (int i = 100; i < 100 + counter; i++) {
        command += "gifTemp/level_" + std::to_string(i) + fileType + " ";
        cout << command << endl;

    }

    command += gifPath;

    int result = system(command.c_str());

    if (result != 0) {
        cout << "Failed to create GIF. Check if ImageMagick is properly installed and available in PATH." << endl;
    } else {
        cout << "GIF successfully created." << endl;
    }

    
    for(int i = 100; i < 100 + counter; i++){
        if(fileType == ".png"){
            string filename = "gifTemp/level_" + std::to_string(i) + ".png";
            fs::remove(filename);
        }
        // else{
        //     string filename = "gifTemp/level_" + std::to_string(i) + ".jpg";
        //     fs::remove(filename);
        // }
    }

    uintmax_t compressedFileSize = fs::file_size(outputPath);

    int depth = quadTree.getDepth(root);
    int nodeCount = quadTree.countNode(root);

    quadTree.deleteTree(root);
    
    chrono::duration<double> duration = end - start;


    // === OUTPUTS ===
    double compressionRatio = 100 * (1.0 - (double)compressedFileSize / originalFileSize);

    cout << "\n=== HASIL EKSEKUSI ===" << endl;
    cout << "Waktu eksekusi: " << duration.count() << " detik" << endl;
    cout << "Ukuran gambar sebelum: "<< originalFileSize << endl;
    cout << "Ukuran gambar setelah: "<< compressedFileSize << endl;
    cout << "Persentase kompresi: "<< compressionRatio << "%" << endl;
    cout << "Kedalaman pohon: " << depth << endl;
    cout << "Banyak simpul pada pohon: " << nodeCount << endl;

    return 0;
}
