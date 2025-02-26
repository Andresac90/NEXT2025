#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Ball.h"

/*
  The Projectile class handles the bullets of the Shooter Enemy.
  It can be extended to different projectile types if desired.
*/

class Projectile
{
public:
    Projectile(float x, float y, float vx, float vy, float radius);

    void Update(float deltaTime);
    void Render() const;
    bool CheckCollision(const Ball& ball) const;

    bool IsActive() const { return active; }
    void Deactivate() { active = false; }

private:
    float x, y;    //World position
    float vx, vy;  //Velocity
    float radius;
    bool active;   //Once off-screen or collided => false
};

#endif
