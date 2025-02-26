#include "stdafx.h"
#include "Wall.h"
#include "app/app.h"
#include <cmath>

Wall::Wall(float x1, float y1, float x2, float y2)
    : x1(x1), y1(y1), x2(x2), y2(y2)
{
}

void Wall::Render() const
{
    //Draw a green line to represent the wall
    App::DrawLine(x1, y1, x2, y2, 0.f, 1.f, 0.f);
}

bool Wall::HandleCollision(Ball& ball)
{
    return CheckCollisionAndReflect(ball);
}

bool Wall::CheckCollisionAndReflect(Ball& ball)
{
    float bx = ball.GetX();
    float by = ball.GetY();
    float br = ball.GetRadius();

    //Vector for the wall
    float wx = x2 - x1;
    float wy = y2 - y1;
    float lenSq = wx * wx + wy * wy;
    if (lenSq < 0.0001f) return false;

    //Project ball center onto segment
    float t = ((bx - x1) * wx + (by - y1) * wy) / lenSq;
    if (t < 0.f) t = 0.f; else if (t > 1.f) t = 1.f;

    float cx = x1 + t * wx;
    float cy = y1 + t * wy;

    float dx = bx - cx;
    float dy = by - cy;
    float distSq = dx * dx + dy * dy;
    float rSq = br * br;

    if (distSq <= rSq)
    {
        //Reflect velocity
        float dist = std::sqrt(distSq);
        if (dist < 0.0001f) dist = br;

        float nx = dx / dist;
        float ny = dy / dist;

        float vx = ball.GetVelocityX();
        float vy = ball.GetVelocityY();

        float dot = vx * nx + vy * ny;
        vx -= 2.f * dot * nx;
        vy -= 2.f * dot * ny;

        ball.SetVelocity(vx, vy);

        //Move ball out
        float overlap = br - dist;
        bx += nx * overlap;
        by += ny * overlap;
        ball.SetPosition(bx, by);

        return true;
    }
    return false;
}
