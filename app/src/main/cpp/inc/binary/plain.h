//
// Created by jinju on 6/14/2020.
//

#ifndef OPENGLES_HW3_PLAIN_H
#define OPENGLES_HW3_PLAIN_H

#include "global.h"
#include "texel.h"

vector<Vertex> plainVertices = {
        {{200.0f, -100.0f, 200.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-200.0f, -100.0f, 200.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 1.0f}},
        {{-200.0f, -100.0f, -200.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, -1.0f}},
        {{200.0f, -100.0f, -200.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, -1.0f}},
};

vector<Index> plainIndices = {
        0, 1, 2,
        0, 2, 3
};

#endif //OPENGLES_HW3_PLAIN_H
