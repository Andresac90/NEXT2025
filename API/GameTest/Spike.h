#ifndef SPIKE_H
#define SPIKE_H

#include "Ball.h"


//Spike is a simple "hazard" entity. This class contains it position, radius and it checks the collision with the ball.


class Spike
{
public:
    Spike(float x, float y, float radius);

    void Render() const;
    bool CheckCollision(const Ball& ball) const;

private:
    float x, y;
    float radius;
};

#endif
