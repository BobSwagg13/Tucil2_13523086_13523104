#ifndef FILEPROCESSING_HPP
#define FILEPROCESSING_HPP

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>


class FileProcessing {
    public:
        struct Image {
            int width;
            int height;
            int channels;
            std::vector<std::vector<std::vector<double>>> data; 
        };
    
        static Image loadImageAsDouble(const std::string& filename);
        static void printImage(const Image& image);
    };


#endif