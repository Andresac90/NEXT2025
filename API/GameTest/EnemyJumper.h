#ifndef ENEMY_JUMPER_H
#define ENEMY_JUMPER_H

#include "Ball.h"

class EnemyJumper
{
public:
    EnemyJumper(float x, float floorY, float jumpHeight, float radius);

    void Update(float deltaTime);
    void Render() const;
    bool CheckCollision(const Ball& ball) const;

private:
    float x, y;
    float floorY;
    float jumpHeight;
    float radius;
    float velocityY;
    bool goingUp;
};

#endif
