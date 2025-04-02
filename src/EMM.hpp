#ifndef EMM_HPP
#define EMM_HPP

#include <vector>
#include <cmath>

double average(const std::vector<std::vector<std::vector<double>>>& img, int channel, int x, int y, int width, int height);
double variance(const std::vector<std::vector<std::vector<double>>>& img, int x, int y, int width, int height);
double meanAbsoluteDeviation(const std::vector<std::vector<std::vector<double>>>& img, int x, int y, int width, int height);
double maxPixelDifference(const std::vector<std::vector<std::vector<double>>>& img, int x, int y, int width, int height);
double entropy(const std::vector<std::vector<std::vector<double>>>& img, int x, int y, int width, int height);
double structuralSimilarityIndex(const std::vector<std::vector<std::vector<double>>>& img1, const std::vector<std::vector<std::vector<double>>>& img2);
#endif
