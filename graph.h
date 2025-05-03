#pragma once

#include "common.h"
#include "node.h"
#include "map.h"

// bool cmp(const std::pair<float, Node*>& b1, const std::pair<float, Node*>& b2) {
//     return b1.first < b2.first;
// }

void kNearestNeighbor(std::vector<Node*>& points, int k, const Map& map) {
    for (Node* p : points) {
        std::vector<std::pair<float, Node*>> distances;
        for (Node* temp : points) {
            if (temp != p) {
                // check if line collided with obstacles
                if (!checkCollisionLineMap(p->pos, temp->pos, map)) {
                    float dis = distance(p->pos, temp->pos);
                    distances.push_back({dis, temp});
                }
            }
        }

        std::sort(distances.begin(), distances.end(), [] (const std::pair<float, Node*>& b1, const std::pair<float, Node*>& b2) {
            return b1.first < b2.first;
        });
        if (distances.size() < k) {
            for (int i = 0; i < distances.size(); i++) {
                p->children.push_back(distances[i].second);
                distances[i].second->children.push_back(p);
            }
        } else {
            for (int i = 0; i < k; i++) {
                p->children.push_back(distances[i].second);
                distances[i].second->children.push_back(p);
            }
        }
    }
}

void drawGraph(std::vector<Node*>& points, float alpha) {
    for (Node* p : points) {
        for (Node* child : p->children) {
            DrawLineV(p->pos, child->pos, Fade(BLACK, alpha));
        }   
    }

    for (Node* p : points) DrawCircle(p->pos.x, p->pos.y, 5, BLUE);
}
