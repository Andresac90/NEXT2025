#include "stdafx.h"
#include "Spike.h"
#include "app/app.h"
#include <cmath>

Spike::Spike(float x, float y, float r)
    : x(x), y(y), radius(r)
{
}

void Spike::Render() const
{
    // Red X Shape for the Spike
    App::DrawLine(x - radius, y - radius, x + radius, y + radius, 1.f, 0.f, 0.f);
    App::DrawLine(x - radius, y + radius, x + radius, y - radius, 1.f, 0.f, 0.f);
}

bool Spike::CheckCollision(const Ball& ball) const
{
    float dx = ball.GetX() - x;
    float dy = ball.GetY() - y;
    float distSq = dx * dx + dy * dy;
    float collideDist = ball.GetRadius() + radius;
    return (distSq <= collideDist * collideDist);
}
