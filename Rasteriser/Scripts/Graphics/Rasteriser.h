#pragma once
#include <vector>

#include "../Core/FloatTypes.h"

struct Tile
{
    int minX, maxX;
    int minY, maxY;
    std::vector<int> triangles;
};

struct ScreenTri
{
    float3 a, b, c;
    int triIndex;
};