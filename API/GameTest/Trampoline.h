#ifndef TRAMPOLINE_H
#define TRAMPOLINE_H

#include "Ball.h"
#include <vector>

/*
   The Trampoline class simulates interconnected springs
   forming a dynamic line. It provides realistic physics-based bouncing
   and a wave effect when the ball interacts with it.
*/

struct SpringPoint
{
    float x;
    float y;
    float restY;
    float velocity;
};

class Trampoline
{
public:
    Trampoline(float x, float y, float length, float intensity, float orientationDeg = 0.f);

    void Init();
    void Update(float deltaTime);
    void Render() const;

    bool HandleCollision(Ball& ball);

    void ResetBounceThisLevel() { hasBouncedThisLevel = false; }
    bool DidReEnableShootingOnBounce() const { return reEnableShooting; }
    void ResetReEnableShootingFlag() { reEnableShooting = false; }

private:
    float x, y;
    float length;
    float intensity; //Bounce force multiplier
    float orientationDeg;

    float springConstant; //Controls the stiffness of the springs
    float damping; //Dampens spring motion
    float propagationSpeed;

    bool hasBouncedThisLevel;
    bool reEnableShooting;

    std::vector<SpringPoint> points;

    void GenerateRipple(int pointIndex, float force);
};

#endif
