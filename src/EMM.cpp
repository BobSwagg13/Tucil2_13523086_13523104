#include "EMM.hpp"

using namespace std;

double average(const vector<vector<vector<double>>>& img, int channel) {
    int N = img.size() * img[0].size();
    double sum = 0;
    for(int i = 0; i < img.size(); i++) {
        for(int j = 0; j < img[0].size(); j++) {
            sum += img[i][j][channel];
        }
    }
    return sum / N;
}

double variance(const vector<vector<vector<double>>>& img) {
    int N = img.size() * img[0].size(); //banyak pixel
    double avgR = average(img, 0);
    double avgG = average(img, 1);
    double avgB = average(img, 2);

    double sumR = 0;
    double sumG = 0;
    double sumB = 0;

    for(int i = 0; i < img.size(); i++) {
        for(int j = 0; j < img[0].size(); j++) {
            sumR += pow(img[i][j][0] - avgR, 2);
            sumG += pow(img[i][j][1] - avgG, 2);
            sumB += pow(img[i][j][2] - avgB, 2);
        }
    }

    double varR = sumR / N;
    double varG = sumG / N;
    double varB = sumB / N;

    return (varR + varG + varB) / 3;
}

double meanAbsoluteDeviation(const vector<vector<vector<double>>>& img) {
    int N = img.size() * img[0].size(); //banyak pixel
    double avgR = average(img, 0);
    double avgG = average(img, 1);
    double avgB = average(img, 2);

    double sumR = 0;
    double sumG = 0;
    double sumB = 0;

    for(int i = 0; i < img.size(); i++) {
        for(int j = 0; j < img[0].size(); j++) {
            sumR += abs(img[i][j][0] - avgR);
            sumG += abs(img[i][j][1] - avgG);
            sumB += abs(img[i][j][2] - avgB);
        }
    }

    double madR = sumR / N;
    double madG = sumG / N;
    double madB = sumB / N;

    return (madR + madG + madB) / 3;
}

double maxPixelDifference(const vector<vector<vector<double>>>& img) {
    double maxR = 0;
    double maxG = 0;
    double maxB = 0;

    double minR = 255;
    double minG = 255;
    double minB = 255;

    for(int i = 0; i < img.size(); i++) {
        for(int j = 0; j < img[0].size(); j++) {
            if(img[i][j][0] > maxR) {
                maxR = img[i][j][0];
            }
            if(img[i][j][1] > maxG) {
                maxG = img[i][j][1];
            }
            if(img[i][j][2] > maxB) {
                maxB = img[i][j][2];
            }  

            if(img[i][j][0] < minR) {
                minR = img[i][j][0];
            }  
            if(img[i][j][1] < minG) {
                minG = img[i][j][1];
            }
            if(img[i][j][2] < minB) {
                minB = img[i][j][2];
            }                                                                                                         
        }
    }
                                                                                                        
    return (maxR + maxG + maxB - minR - minG - minB) / 3;
}

double entropy(const vector<vector<vector<double>>>& img) {
    int N = img.size() * img[0].size(); //banyak pixel
    double sumR = 0;
    double sumG = 0;
    double sumB = 0;

    double histR[256] = {0};
    double histG[256] = {0};
    double histB[256] = {0};

    for(int i = 0; i < img.size(); i++) {
        for(int j = 0; j < img[0].size(); j++) {
            histR[(int)img[i][j][0]]++;
            histG[(int)img[i][j][1]]++;
            histB[(int)img[i][j][2]]++;
        }
    }

    for(int i = 0; i < 256; i++) {
        if(histR[i] != 0) {
            sumR += histR[i] * log2(histR[i]);
        }
        if(histG[i] != 0) {
            sumG += histG[i] * log2(histG[i]);
        }
        if(histB[i] != 0) {
            sumB += histB[i] * log2(histB[i]);
        }
    }

    double entropyR = -sumR/N;
    double entropyG = -sumG/N;
    double entropyB = -sumB/N;

    return (entropyR + entropyG + entropyB) / 3;
}

double structuralSimilarityIndex(const vector<vector<vector<double>>>& img1, const vector<vector<vector<double>>>& img2) {
    
    //TODO: Implementasi SSIM
    return 0;
}