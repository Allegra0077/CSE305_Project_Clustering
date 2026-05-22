// sequential Boruvka's algorithm for the Euclidean MST of a point set
#pragma once

#include "common/point.h"
#include <vector>

// an edge in the MST: connects points u and v with weight = euclidean(p_u, p_v)
struct MSTEdge {
    int u;
    int v;
    double weight;
};

// computes the Euclidean Minimum Spanning Tree of points using Boruvka's algo
// returns the N-1 edges of the MST 
std::vector<MSTEdge> boruvka_mst_sequential(const std::vector<Point>& points);