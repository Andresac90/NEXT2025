#include "stdafx.h"
#include "EnemyJumper.h"
#include "app/app.h"
#include <cmath>

EnemyJumper::EnemyJumper(float x, float floorY, float jumpHeight, float radius)
    : x(x), y(floorY), floorY(floorY), jumpHeight(jumpHeight),
    radius(radius), velocityY(0.f), goingUp(true)
{
}

void EnemyJumper::Update(float deltaTime)
{
    float dt = deltaTime * 0.001f;
    //Simple up/down logic
    if (goingUp)
    {
        velocityY = 100.f;
        y += velocityY * dt;
        if (y >= floorY + jumpHeight)
        {
            y = floorY + jumpHeight;
            goingUp = false;
        }
    }
    else
    {
        velocityY = -100.f;
        y += velocityY * dt;
        if (y <= floorY)
        {
            y = floorY;
            goingUp = true;
        }
    }
}

void EnemyJumper::Render() const
{
    //Purple X shape for the enemy
    App::DrawLine(x - radius, y - radius, x + radius, y + radius, 1.f, 0.f, 1.f);
    App::DrawLine(x - radius, y + radius, x + radius, y - radius, 1.f, 0.f, 1.f);
}

bool EnemyJumper::CheckCollision(const Ball& ball) const
{
    float dx = ball.GetX() - x;
    float dy = ball.GetY() - y;
    float distSq = dx * dx + dy * dy;
    float combined = ball.GetRadius() + radius;
    return (distSq <= combined * combined);
}
