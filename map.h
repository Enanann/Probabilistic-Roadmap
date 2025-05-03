#pragma once

#include "common.h"
#include "node.h"

class Map {
public:
    std::vector<Rectangle> obstaclesRec;

    Map(std::vector<Rectangle> o1) : obstaclesRec(o1) {}
};

bool checkCollisionPointRec(Vector2 center, float radius, Rectangle rec) {
    float closetX = fmax(rec.x, fmin(center.x, rec.x + rec.width));
    float closetY = fmax(rec.y, fmin(center.y, rec.y + rec.height));
    
    return ((closetX - center.x) * (closetX - center.x) + (closetY - center.y) * (closetY - center.y)) <= radius * radius;
}

bool isOverlapping(Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
    // horizontal
    if ((float)(a.y - b.y) < 1e-6 && (float)(b.y - c.y) < 1e-6 && (float)(c.y - d.y) < 1e-6) {
        float abMin = fminf(a.x, b.x);
        float abMax = fmaxf(a.x, b.x);
        float cdMin = fminf(c.x, d.x);
        float cdMax = fmaxf(c.x, d.x);

        return !(abMax < cdMin || cdMax < abMin);
    } else { // vertical
        float abMin = fminf(a.y, b.y);
        float abMax = fmaxf(a.y, b.y);
        float cdMin = fminf(c.y, d.y);
        float cdMax = fmaxf(c.y, d.y);

        return !(abMax < cdMin || cdMax < abMin);
    }
}

bool checkCollisionLineLine(Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
    float denominator = (d.y - c.y) * (b.x - a.x) - (d.x - c.x) * (b.y - a.y);
    float ua = ((d.x - c.x) * (a.y - c.y) - (d.y - c.y) * (a.x - c.x));
    float ub = ((b.x - a.x) * (a.y - c.y) - (b.y - a.y) * (a.x - c.x));

    if (fabsf(denominator) < 1e-6) {
        if (fabsf(ua) < 1e-6 && fabsf(ub) < 1e-6) return isOverlapping(a, b, c, d);
        return false; // heikousei rip
    }

    ua /= denominator;
    ub /= denominator;
    
    return (0 <= ua && ua <= 1) && (0 <= ub && ub <= 1); // true => intersect 
}

bool checkCollisionPointMap(Vector2 point, const Map& map) {
    for (Rectangle r : map.obstaclesRec) {
        if (checkCollisionPointRec(point, 5, r)) {
            return true;
        }
    }
    return false;
}


bool checkCollisionLineMap(Vector2 a, Vector2 b, const Map& map) {
    for (Rectangle r : map.obstaclesRec) {
        if (checkCollisionLineLine(a, b, {r.x, r.y}, {r.x + r.width, r.y}) ||  // Top
            checkCollisionLineLine(a, b, {r.x, r.y}, {r.x, r.y + r.height}) || // Left
            checkCollisionLineLine(a, b, {r.x + r.width, r.y}, {r.x + r.width, r.y + r.height}) || // Right
            checkCollisionLineLine(a, b, {r.x, r.y + r.height}, {r.x + r.width, r.y + r.height}))  // Bottom
        {
            return true;  
        }
    }
    return false;
}
