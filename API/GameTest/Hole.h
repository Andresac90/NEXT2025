#ifndef HOLE_H
#define HOLE_H

#include "Ball.h"

/*
  The Hole class contains the position and radius of the golf hole and
  it checks the collision with the ball.
*/

class Hole
{
public:
    Hole(float x = 0.f, float y = 0.f, float radius = 15.f);

    void Render() const;
    bool CheckCollision(const Ball& ball) const;

private:
    float x, y;
    float radius;
};

#endif 
