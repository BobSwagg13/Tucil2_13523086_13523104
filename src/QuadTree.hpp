#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <vector>
#include <algorithm>
#include <functional>
#include <queue>

class QuadTree {
    public:
        struct Node {
            int x;
            int y;
            int width;
            int height;
            Node* topLeft;
            Node* topRight;
            Node* bottomLeft;
            Node* bottomRight;
        };

        QuadTree();
        ~QuadTree();

        static Node* buildTree(const std::vector<std::vector<std::vector<double>>>& img, int x, int y, int width, int height);
        static void divideTree(Node* node, const std::vector<std::vector<std::vector<double>>>& img, int method, double threshold, int minBlockSize);
        static void nodeToMatrix(Node* node, std::vector<std::vector<std::vector<double>>>& img);
        static void printTree(Node* node);
        static int countNode(Node* node);
        static int getDepth(Node* node);
        static void deleteTree(Node* node);
        static void bfsAveragePerLevelImage(Node* node, const std::vector<std::vector<std::vector<double>>>& img, std::function<void(const std::vector<std::vector<std::vector<double>>>&, int)> handleLevelImag);
        static void avgTheRGB_fromOriginal(const Node* node,std::vector<std::vector<std::vector<double>>>& targetImg,const std::vector<std::vector<std::vector<double>>>& sourceImg);
    };  
#endif