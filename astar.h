#pragma once

#include "common.h"
#include "node.h"

float heuristic(Node* node, Node* goal);

class AStarData {
public:
    std::unordered_map<Node*, float> gScore; // cost from start to node n
    std::unordered_map<Node*, float> fScore; // heuristic cost from node n to goal
    std::unordered_map<Node*, Node*> cameFrom;
    
    
    void initialize(std::vector<Node*>& allNodes, Node* start, Node* goal) {
        gScore.clear();
        fScore.clear();
        cameFrom.clear();
        for (Node* node : allNodes) {
            gScore[node] = INFINITY;
            fScore[node] = INFINITY;
        }
        gScore[start] = (float)0;
        fScore[start] = heuristic(start, goal);
    }
};

class CompareFScore {
public:
    std::unordered_map<Node*, float>& fScore;

    CompareFScore(std::unordered_map<Node*, float>& fScore) : fScore(fScore) {}

    bool operator()(Node* a, Node* b) const {
        return fScore[a] > fScore[b];
    }
};

float heuristic(Node* node, Node* goal) {
    return distance(node->pos, goal->pos);
}

std::vector<Node*> reconstructPath(Node* start, Node* goal, const std::unordered_map<Node*, Node*>& cameFrom) {
    std::vector<Node*> path;
    
    if (cameFrom.find(goal) == cameFrom.end()) return path;

    Node* current = goal;
    while (current != start) {
        path.push_back(current);
        current = cameFrom.at(current);
    }

    path.push_back(start);
    std::reverse(path.begin(), path.end()); 

    return path;
}

void aStar(AStarData& asd, Node* start, Node* goal, const std::vector<Node*> allNodes) {
    CompareFScore cmp(asd.fScore);
    std::priority_queue<Node*, std::vector<Node*>, CompareFScore> q(cmp);
    q.push(start);

    while (!q.empty()) {
        Node* current = q.top();
        q.pop();

        if (current == goal) {
            break;
        }

        for (Node* neighbor : current->children) {
            float tentativeG = asd.gScore[current] + distance(current->pos, neighbor->pos);

            if (tentativeG < asd.gScore[neighbor]) {
                asd.cameFrom[neighbor] = current;
                asd.gScore[neighbor] = tentativeG;
                asd.fScore[neighbor] = tentativeG + heuristic(neighbor, goal);

                q.push(neighbor);
            }
        }
    }

    if (asd.cameFrom.find(goal) == asd.cameFrom.end()) {
        std::cout << "no path found!\n";
    }
}

void drawPath(std::vector<Node*> path) {
    for (int i = 0; i < path.size() - 1; i++) {
        Node* p1 = path[i];
        Node* p2 = path[i + 1];
        DrawLineEx(p1->pos, p2->pos, 4, GREEN);
    }
    for (auto b : path) {
        DrawCircle(b->pos.x, b->pos.y, 5, DARKGREEN);
    }
}
