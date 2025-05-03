#pragma once

#include "common.h"
#include "map.h"

class Node {
public:
    Vector2 pos;
    std::vector<Node*> children;

    Node() : pos() {}

    Node(Vector2 pos) : pos(pos) {}
};
    
float distance(Vector2 b1, Vector2 b2) {
    return sqrt(pow(b2.x - b1.x, 2) + pow(b2.y - b1.y, 2));
}

int RNGX(float X, float Y) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(X + 5, Y - 5); // for radius
    return dist(gen);
}

int RNGY(float X, float Y) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(X + 5, Y - 5); // for radius
    return dist(gen);
}

// Graph
void createSamplePoint(std::vector<Node*>& points, const Map& map, Rectangle scr) {
    Vector2 temp = {(float)RNGX(scr.x, scr.x + scr.width), (float)RNGY(scr.y, scr.y + scr.height)};
    
    if (!checkCollisionPointMap(temp, map)) {
        // std::cout << "Collided!\n";
        Node* newNode = new Node(temp);
        points.push_back(newNode);
    }
}

// Delete all nodes (quit)
void deletePoints(std::vector<Node*>& points) {
    for (Node* p : points) {
        delete p;  
    }
    points.clear();
}

// Delete all nodes except start, goal (reset)
void deletePoints(std::vector<Node*>& points, Node*& root, Node*& goal) { 
    for (Node* p : points) {
        if (p != root && p != goal) {
            delete p;  
        } 
    }
    points.clear();
    
    if (root) {
        root->children.clear();
        points.push_back(root);
    }
    
    if (goal) {
        goal->children.clear();
        points.push_back(goal);
    }

}
