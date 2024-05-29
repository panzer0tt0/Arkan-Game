//
// Created by Francesco on 18/05/2024.
//

#ifndef ARKAN0_1_BLOCK_H
#define ARKAN0_1_BLOCK_H

#endif //ARKAN0_1_BLOCK_H

#pragma once

#include "raylib.h"
#include <cmath>


struct Block {
    Vector2 DIMENSIONS = {SCREEN_WIDTH/10, 25};
    Vector2 location;
    bool isActive = true;

    enum collisionWithBlock {notCollided = 0, topCollision = 1, bottomCollision, leftCollision, rightCollision};
};