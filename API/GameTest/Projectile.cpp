#include "stdafx.h"
#include "Projectile.h"
#include "app/app.h"
#include <cmath>

Projectile::Projectile(float x, float y, float vx, float vy, float radius)
    : x(x), y(y), vx(vx), vy(vy), radius(radius), active(true)
{
}

void Projectile::Update(float deltaTime)
{
    if (!active) return;

    float dt = deltaTime * 0.001f;
    x += vx * dt;
    y += vy * dt;

    // Deactivate if leaving bounds
    if (x < -100.f || x > 2000.f || y < -100.f || y > 1200.f)
    {
        active = false;
    }
}

void Projectile::Render() const
{
    if (!active) return;

    // Draw a yellow X
    App::DrawLine(x - radius, y - radius, x + radius, y + radius, 1.f, 1.f, 0.f);
    App::DrawLine(x - radius, y + radius, x + radius, y - radius, 1.f, 1.f, 0.f);
}

bool Projectile::CheckCollision(const Ball& ball) const
{
    if (!active) return false;

    float dx = ball.GetX() - x;
    float dy = ball.GetY() - y;
    float distSq = dx * dx + dy * dy;
    float sumR = ball.GetRadius() + radius;
    return distSq <= (sumR * sumR);
}
