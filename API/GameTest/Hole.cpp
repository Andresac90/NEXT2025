#include "stdafx.h"
#include "Hole.h"
#include "app/app.h"
#include <cmath>

Hole::Hole(float x, float y, float r)
    : x(x), y(y), radius(r)
{
}

void Hole::Render() const
{
    App::DrawLine(x - radius, y, x + radius, y, 1.f, 1.f, 0.f);
    App::DrawLine(x, y - radius, x, y + radius, 1.f, 1.f, 0.f);
}

bool Hole::CheckCollision(const Ball& ball) const
{
    float dx = ball.GetX() - x;
    float dy = ball.GetY() - y;
    float distSq = dx * dx + dy * dy;
    float sumRadius = ball.GetRadius() + radius;
    return (distSq <= sumRadius * sumRadius);
}
