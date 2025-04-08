#include "EMM.hpp"

using namespace std;

double average(const vector<vector<vector<double>>>& img, int channel, int x, int y, int width, int height) {
    int N = width * height; //banyak pixel
    double sum = 0;
    for(int i = y; i < y + height; i++) {
        for(int j = x; j < x + width; j++) {
            sum += img[i][j][channel];
        }
    }
    return sum / N;
}

double standardDeviation(const vector<vector<vector<double>>>& img, int channel, int x, int y, int width, int height) {
    double avg = average(img, channel, x, y, width, height);
    double sum = 0;
    for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
            sum += pow(img[i][j][channel] - avg, 2);
        }
    }

    double stdDev = sqrt(sum / ((width * height) - 1));

    return stdDev;
}

double variance(const vector<vector<vector<double>>>& img, int x, int y, int width, int height) {
    int N = width * height;

    double sumR = 0, sumG = 0, sumB = 0;

    for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
            sumR += img[i][j][0];
            sumG += img[i][j][1];
            sumB += img[i][j][2];
        }
    }

    double avgR = sumR / N;
    double avgG = sumG / N;
    double avgB = sumB / N;

    double varR = 0, varG = 0, varB = 0;

    for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
            varR += pow(img[i][j][0] - avgR, 2);
            varG += pow(img[i][j][1] - avgG, 2);
            varB += pow(img[i][j][2] - avgB, 2);
        }
    }

    return (varR + varG + varB) / (3 * N);
}


double meanAbsoluteDeviation(const vector<vector<vector<double>>>& img, int x, int y, int width, int height) {
    int N = width * height;

    double avgR = average(img, 0, x, y, width, height);
    double avgG = average(img, 1, x, y, width, height);
    double avgB = average(img, 2, x, y, width, height);

    double sumR = 0, sumG = 0, sumB = 0;

    for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
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


double maxPixelDifference(const vector<vector<vector<double>>>& img, int x, int y, int width, int height) {
    double maxR = 0, maxG = 0, maxB = 0;
    double minR = 255, minG = 255, minB = 255;

    for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
            double r = img[i][j][0];
            double g = img[i][j][1];
            double b = img[i][j][2];

            if (r > maxR) maxR = r;
            if (g > maxG) maxG = g;
            if (b > maxB) maxB = b;

            if (r < minR) minR = r;
            if (g < minG) minG = g;
            if (b < minB) minB = b;
        }
    }

    return (maxR - minR + maxG - minG + maxB - minB) / 3.0;
}


double entropy(const vector<vector<vector<double>>>& img, int x, int y, int width, int height) {
    int N = width * height;

    double histR[256] = {0}, histG[256] = {0}, histB[256] = {0};

    for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
            histR[(int)img[i][j][0]]++;
            histG[(int)img[i][j][1]]++;
            histB[(int)img[i][j][2]]++;
        }
    }

    double sumR = 0, sumG = 0, sumB = 0;

    for (int i = 0; i < 256; i++) {
        if (histR[i] > 0) sumR += histR[i] * log2(histR[i] / N);
        if (histG[i] > 0) sumG += histG[i] * log2(histG[i] / N);
        if (histB[i] > 0) sumB += histB[i] * log2(histB[i] / N);
    }

    double entropyR = -sumR;
    double entropyG = -sumG;
    double entropyB = -sumB;

    return (entropyR + entropyG + entropyB) / (3 * N);
}


double structuralSimilarityIndex(const vector<vector<vector<double>>>& img, int x, int y, int width, int height) {
    double stdDevR = standardDeviation(img, 0, x, y, width, height);
    double stdDevG = standardDeviation(img, 1, x, y, width, height);
    double stdDevB = standardDeviation(img, 2, x, y, width, height);

    double c1 = 6.5025, c2 = 58.5225; //c1 =   (0.01 * 255) ^ 2, c2 = (0.03 * 255) ^ 2
    double ssimR = 0, ssimG = 0, ssimB = 0;
    ssimR = c2 / (stdDevR * stdDevR + c2); //rumus cepet angjayy
    ssimG = c2 / (stdDevG * stdDevG + c2);
    ssimB = c2 / (stdDevB * stdDevB + c2);

    return 0.299 * ssimR + 0.587 * ssimG + 0.114 * ssimB;
}