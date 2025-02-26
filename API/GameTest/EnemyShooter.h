#ifndef ENEMY_SHOOTER_H
#define ENEMY_SHOOTER_H

#include <vector>
#include "Ball.h"
#include "Projectile.h"

class EnemyShooter
{
public:
    EnemyShooter(float x, float y, float fireRate, float projectileSpeed, float radius);

    void Update(float deltaTime, const Ball& ball);
    void Render() const;

    bool CheckProjectilesCollision(Ball& ball);

private:
    float x, y;
    float radius;
    float fireRate;        //Seconds between shots
    float projectileSpeed;
    float fireTimer;       //Accumulates time

    std::vector<Projectile> projectiles;
};

#endif
