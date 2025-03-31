#ifndef EMM_HPP
#define EMM_HPP

#include <vector>
#include <cmath>
#include <iostream>

static double average(const std::vector<std::vector<std::vector<double>>>& img, int channel);
static double variance(const std::vector<std::vector<std::vector<double>>>& img);
static double meanAbsoluteDeviation(const std::vector<std::vector<std::vector<double>>>& img);
static double maxPixelDifference(const std::vector<std::vector<std::vector<double>>>& img);
static double entropy(const std::vector<std::vector<std::vector<double>>>& img);
static double structuralSimilarityIndex(const std::vector<std::vector<std::vector<double>>>& img1, const std::vector<std::vector<std::vector<double>>>& img2);
#endif
