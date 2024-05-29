//
// Created by Francesco on 18/05/2024.
//

#ifndef ARKAN0_1_BALL_H
#define ARKAN0_1_BALL_H

#endif //ARKAN0_1_BALL_H

#pragma once

#include "block.hpp"
#include "../functions.hpp"
#include <stdexcept>

struct Ball {
    const float DIAMETER = 20;
    const float RADIUS = DIAMETER/2;
    Vector2 location = {100, 100};

    Vector2 speed = {0, 0};


    bool hasBouncedOnPlayer = false;
    bool hasBouncedOnWallRx = false;
    bool hasBouncedOnWallLx = false;
    bool hasBouncedOnWallTop = false;
    bool hasBouncedOnBlock = false;



    void setInitSpeed(){
        speed = {2, -2};
    }

    void move(){
        location.x += speed.x;
        location.y += speed.y;
    }

    int collisionWithBlock(Block block) {


        if(CheckCollisionCircleRec(location, RADIUS, (Rectangle){block.location.x, block.location.y, block.DIMENSIONS.x, block.DIMENSIONS.y})) {// Check if the ball is colliding with the top side of the block
            if (location.y + RADIUS >= block.location.y && (speed.y > 0)) {
                double distToTopEdge = distance(location.x, location.y + RADIUS, location.x, block.location.y);
                if (distToTopEdge < RADIUS &&
                    distToTopEdge < distance(location.x + RADIUS, location.y, block.location.x, location.y) &&
                    distToTopEdge <
                    distance(location.x - RADIUS, location.y, block.location.x + block.DIMENSIONS.x, location.y)) {
                    return 1;
                }
            }

            // Check if the ball is colliding with the bottom side of the block
            if (location.y - RADIUS <= block.location.y + block.DIMENSIONS.y && (speed.y < 0)) {
                double distToBottomEdge = distance(location.x, location.y - RADIUS, location.x,
                                                   block.location.y + block.DIMENSIONS.y);
                if (distToBottomEdge < RADIUS &&
                    distToBottomEdge < distance(location.x + RADIUS, location.y, block.location.x, location.y) &&
                    distToBottomEdge <
                    distance(location.x - RADIUS, location.y, block.location.x + block.DIMENSIONS.x, location.y)) {
                    return 2;
                }
            }

            // Check if the ball is colliding with the left side of the block
            if (location.x + RADIUS >= block.location.x && (speed.x > 0)) {
                double distToLeftEdge = distance(location.x + RADIUS, location.y, block.location.x, location.y);
                if (distToLeftEdge < RADIUS &&
                    distToLeftEdge < distance(location.x, location.y + RADIUS, location.x, block.location.y) &&
                    distToLeftEdge <
                    distance(location.x, location.y - RADIUS, location.x, block.location.y + block.DIMENSIONS.y)) {
                    return 3;
                }
            }

            // Check if the ball is colliding with the right side of the block
            if (location.x - RADIUS <= block.location.x + block.DIMENSIONS.x && (speed.x < 0)) {
                double distToRightEdge = distance(location.x - RADIUS, location.y,
                                                  block.location.x + block.DIMENSIONS.x, location.y);
                if (distToRightEdge < RADIUS &&
                    distToRightEdge < distance(location.x, location.y + RADIUS, location.x, block.location.y) &&
                    distToRightEdge <
                    distance(location.x, location.y - RADIUS, location.x, block.location.y + block.DIMENSIONS.y)) {
                    return 4;
                }
            }
        }

        // The ball is not colliding with the block
        return 0;
    }

    void resetBouncesFlags(){
        hasBouncedOnPlayer = false;
        hasBouncedOnWallRx = false;
        hasBouncedOnWallLx = false;
        hasBouncedOnWallTop = false;
        hasBouncedOnBlock = false;
    }

    enum bouncesTypes {onPlayer = 1, onWallRx, onWallLx, onWallTop, onBlock};

    void setBouncesFlags(int bounceType){
        switch (bounceType) {
            case onPlayer:
                hasBouncedOnPlayer = true;
                hasBouncedOnWallRx = false;
                hasBouncedOnWallLx = false;
                hasBouncedOnWallTop = false;

                break;
            case onWallRx:
                hasBouncedOnPlayer = false;
                hasBouncedOnWallRx = true;
                hasBouncedOnWallLx = false;
                hasBouncedOnWallTop = false;

                break;
            case onWallLx:
                hasBouncedOnPlayer = false;
                hasBouncedOnWallRx = false;
                hasBouncedOnWallLx = true;
                hasBouncedOnWallTop = false;

                break;
            case onWallTop:
                hasBouncedOnPlayer = false;
                hasBouncedOnWallRx = false;
                hasBouncedOnWallLx = false;
                hasBouncedOnWallTop = true;

                break;
            case onBlock:
                hasBouncedOnPlayer = false;
                hasBouncedOnWallRx = false;
                hasBouncedOnWallLx = false;
                hasBouncedOnWallTop = false;
                hasBouncedOnBlock = true;

                break;
            default:
                throw std::invalid_argument("choose a value between 1 and 5");
        }
    }


};
