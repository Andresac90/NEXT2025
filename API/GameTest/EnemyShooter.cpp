#include "stdafx.h"
#include "EnemyShooter.h"
#include "app/app.h"
#include <cmath>

EnemyShooter::EnemyShooter(float x, float y, float fireRate, float projectileSpeed, float radius)
    : x(x), y(y), radius(radius), fireRate(fireRate),
    projectileSpeed(projectileSpeed), fireTimer(0.f)
{
}

void EnemyShooter::Update(float deltaTime, const Ball& ball)
{
    float dt = deltaTime * 0.001f;
    fireTimer += dt;

    //Code for FireRate of the bullets depending on the time defined
    if (fireTimer >= fireRate)
    {
        fireTimer = 0.f;
        //Aim at the ball
        float bx = ball.GetX();
        float by = ball.GetY();
        float dx = bx - x;
        float dy = by - y;
        float len = std::sqrt(dx * dx + dy * dy);
        if (len < 0.0001f) len = 1.f;

        float vx = (dx / len) * projectileSpeed;
        float vy = (dy / len) * projectileSpeed;

        projectiles.emplace_back(x, y, vx, vy, 5.f); //radius=5
    }

    //Update projectiles
    for (auto& p : projectiles)
    {
        p.Update(deltaTime);
    }
}

void EnemyShooter::Render() const
{
    //Purple X shape for the enemy shooter
    App::DrawLine(x - radius, y - radius, x + radius, y + radius, 1.f, 0.f, 1.f);
    App::DrawLine(x - radius, y + radius, x + radius, y - radius, 1.f, 0.f, 1.f);

    for (auto& p : projectiles)
    {
        p.Render();
    }
}

bool EnemyShooter::CheckProjectilesCollision(Ball& ball)
{
    for (auto& p : projectiles)
    {
        if (p.IsActive() && p.CheckCollision(ball))
        {
            p.Deactivate();
            return true;
        }
    }
    return false;
}
