#ifndef WALL_H
#define WALL_H

#include "Ball.h"

//This class contains the data for the wall object and it checks the collision with the ball.

class Wall
{
public:
    Wall(float x1, float y1, float x2, float y2);

    void Render() const;
    bool HandleCollision(Ball& ball);

private:
    float x1, y1;
    float x2, y2;

    bool CheckCollisionAndReflect(Ball& ball);
};

#endif 
