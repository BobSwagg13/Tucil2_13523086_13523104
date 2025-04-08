#include "QuadTree.hpp"
#include "EMM.hpp"
#include <iostream>
#include <vector>

using namespace std;

QuadTree::QuadTree() {}
QuadTree::~QuadTree() {}

QuadTree::Node* QuadTree::buildTree(const vector<vector<vector<double>>>& img, int x, int y, int width, int height) {
    QuadTree::Node* node = new QuadTree::Node();
    node->x = x;
    node->y = y;
    node->width = width;
    node->height = height;
    node->topLeft = nullptr;
    node->topRight = nullptr;
    node->bottomLeft = nullptr;
    node->bottomRight = nullptr;

    return node;
}

void QuadTree::divideTree(QuadTree::Node* node, const vector<vector<vector<double>>>& img, int method, double threshold, int minBlockSize) {
    if (node->width <= minBlockSize || node->height <= minBlockSize) return;
    
    double error;
    switch (method) {
        case 1: error = variance(img, node->x, node->y, node->width, node->height); break;
        case 2: error = meanAbsoluteDeviation(img, node->x, node->y, node->width, node->height); break;
        case 3: error = maxPixelDifference(img, node->x, node->y, node->width, node->height); break;
        case 4: error = entropy(img, node->x, node->y, node->width, node->height); break;
        case 5: error = structuralSimilarityIndex(img, node->x, node->y, node->width, node->height); break;
        default: return;
    }

    if (error <= threshold && method < 5) {return;}
    if (error >= threshold && method == 5) {
        return;
    }

    int w1 = node->width / 2;
    int w2 = node->width - w1;      // ensures w1 + w2 == width
    int h1 = node->height / 2;
    int h2 = node->height - h1;     // ensures h1 + h2 == height

    node->topLeft = buildTree(img, node->x, node->y, w1, h1);
    node->topRight = buildTree(img, node->x + w1, node->y, w2, h1);
    node->bottomLeft = buildTree(img, node->x, node->y + h1, w1, h2);
    node->bottomRight = buildTree(img, node->x + w1, node->y + h1, w2, h2);

    divideTree(node->topLeft, img, method, threshold, minBlockSize);
    divideTree(node->topRight, img, method, threshold, minBlockSize);
    divideTree(node->bottomLeft, img, method, threshold, minBlockSize);
    divideTree(node->bottomRight, img, method, threshold, minBlockSize);
}


void QuadTree::nodeToMatrix(QuadTree::Node* node, vector<vector<vector<double>>>& img) {
    if (node == nullptr) return;

    if (node->topLeft == nullptr) {
        double avgR = 0;
        double avgG = 0;
        double avgB = 0;
        for (int i = node->y; i < node->y + node->height; i++) {
            for (int j = node->x; j < node->x + node->width; j++) {
                avgR += img[i][j][0];
                avgG += img[i][j][1];
                avgB += img[i][j][2];
            }
        }
        avgR /= node->width * node->height;
        avgG /= node->width * node->height;
        avgB /= node->width * node->height;
        for (int i = node->y; i < node->y + node->height; i++) {
            for (int j = node->x; j < node->x + node->width; j++) {
                img[i][j][0] = avgR;
                img[i][j][1] = avgG;
                img[i][j][2] = avgB;
            }
        }
    } else {
        nodeToMatrix(node->topLeft, img);
        nodeToMatrix(node->topRight, img);
        nodeToMatrix(node->bottomLeft, img);
        nodeToMatrix(node->bottomRight, img);
    }
}

void QuadTree::printTree(QuadTree::Node* node) {
    if (node == nullptr) return;

    cout << "Node: (" << node->x << ", " << node->y << ") " << node->width << "x" << node->height << endl;
    printTree(node->topLeft);
    printTree(node->topRight);
    printTree(node->bottomLeft);
    printTree(node->bottomRight);
}

int QuadTree::countNode(QuadTree::Node* node) {
    if (node == nullptr) return 0;

    return 1 + countNode(node->topLeft) + countNode(node->topRight) + countNode(node->bottomLeft) + countNode(node->bottomRight);
}

int QuadTree::getDepth(QuadTree::Node* node) {
    if (node == nullptr) return 0;
    if (!node->topLeft) return 1;
    return 1 + max({getDepth(node->topLeft), getDepth(node->topRight), getDepth(node->bottomLeft), getDepth(node->bottomRight)});
}

void QuadTree::deleteTree(QuadTree::Node* node) {
    if (node == nullptr) return;

    deleteTree(node->topLeft);
    deleteTree(node->topRight);
    deleteTree(node->bottomLeft);
    deleteTree(node->bottomRight);
    delete node;
}

void QuadTree::avgTheRGB_fromOriginal(const Node* node, vector<vector<vector<double>>>& targetImg,const vector<vector<vector<double>>>& sourceImg)
{
    double avgR = 0, avgG = 0, avgB = 0;
    for (int i = node->y; i < node->y + node->height; i++) {
        for (int j = node->x; j < node->x + node->width; j++) {
            avgR += sourceImg[i][j][0];
            avgG += sourceImg[i][j][1];
            avgB += sourceImg[i][j][2];
        }
    }
    int area = node->width * node->height;
    avgR /= area;
    avgG /= area;
    avgB /= area;

    for (int i = node->y; i < node->y + node->height; i++) {
        for (int j = node->x; j < node->x + node->width; j++) {
            targetImg[i][j][0] = avgR;
            targetImg[i][j][1] = avgG;
            targetImg[i][j][2] = avgB;
        }
    }
}

void QuadTree::bfsAveragePerLevelImage(Node* root, const vector<vector<vector<double>>>& originalImg, function<void(const vector<vector<vector<double>>>&, int)> handleLevelImage) {
    if (!root) return;

    std::queue<std::pair<Node*, int>> q;
    q.push({root, 0});
    int currentDepth = 0;

    std::vector<Node*> currentLevelNodes;
    auto currentImg = originalImg;

    while (!q.empty()) {
        Node* node = q.front().first;
        int depth = q.front().second;
        q.pop();

        if (depth != currentDepth) {
            auto imgCopy = currentImg;
            for (Node* n : currentLevelNodes) {
                avgTheRGB_fromOriginal(n, imgCopy, originalImg);
            }

            handleLevelImage(imgCopy, currentDepth);
            currentImg = imgCopy;
                currentLevelNodes.clear();
            currentDepth = depth;
        }
        
        currentLevelNodes.push_back(node);
        if (node->topLeft) {
            q.push({node->topLeft, depth + 1});
            q.push({node->topRight, depth + 1});
            q.push({node->bottomLeft, depth + 1});
            q.push({node->bottomRight, depth + 1});
        }
    }

    if (!currentLevelNodes.empty()) {
        auto imgCopy = currentImg;
        for (Node* n : currentLevelNodes) {
            avgTheRGB_fromOriginal(n, imgCopy, originalImg);
        }
        handleLevelImage(imgCopy, currentDepth);
    }
}