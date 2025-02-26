#ifndef BALL_H
#define BALL_H

#include "app/app.h"

//The Ball class contains: position, velocity, and physics for the ball object.
class Ball
{
public:
    Ball();
    Ball(float initX, float initY, float initRadius);

    void Init();
    void Update(float deltaTime);
    void Render() const;

    //Basic 2D force application
    void ApplyForce(float forceX, float forceY);

    //Accessors and Mutators
    inline void SetPosition(float px, float py) { x = px; y = py; }
    inline void SetVelocity(float vx, float vy) { velocityX = vx; velocityY = vy; }
    inline float GetVelocityX() const { return velocityX; }
    inline float GetVelocityY() const { return velocityY; }

    float GetX() const;
    float GetY() const;
    float GetRadius() const;

private:
    float x, y;             //Position in 2D world
    float velocityX, velocityY;
    float frictionX, frictionY;
    float gravity;          //Vertical gravity
    float radius;           //Collision radius
};

#endif
