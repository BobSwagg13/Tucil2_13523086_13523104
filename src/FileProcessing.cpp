#include "FileProcessing.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

FileProcessing::Image FileProcessing::loadImageAsDouble(const string& filename) {
    int width, height, channels;
    unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!img) {
        throw std::runtime_error("Failed to load image: " + filename);
    }

    FileProcessing::Image image;
    image.width = width;
    image.height = height;
    image.channels = channels;
    image.data.resize(height, std::vector<std::vector<double>>(width, std::vector<double>(channels)));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                int index = (y * width + x) * channels + c;
                image.data[y][x][c] = static_cast<double>(img[index]);
            }
        }
    }

    stbi_image_free(img);
    return image;
}

void FileProcessing::printImage(const FileProcessing::Image& image) {
    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            for (int c = 0; c < image.channels; ++c) {
                cout << image.data[y][x][c] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}