#include "stdafx.h"
#include "Ball.h"
#include <cmath>

Ball::Ball()
    : x(400.f)
    , y(300.f)
    , velocityX(0.f)
    , velocityY(0.f)
    , frictionX(0.98f)
    , frictionY(0.995f)
    , gravity(175.f)
    , radius(10.f)
{
}

Ball::Ball(float initX, float initY, float initRadius)
    : x(initX)
    , y(initY)
    , velocityX(0.f)
    , velocityY(0.f)
    , frictionX(0.98f)
    , frictionY(0.995f)
    , gravity(175.f)
    , radius(initRadius)
{
}

void Ball::Init()
{
    x = 400.f;
    y = 300.f;
    velocityX = 0.f;
    velocityY = 0.f;
}

void Ball::Update(float deltaTime)
{
    float dt = deltaTime * 0.001f;

    //Gravity pulls the ball downward
    velocityY -= gravity * dt;

    //Update position
    x += velocityX * dt;
    y += velocityY * dt;

    //Friction dampens velocity
    velocityX *= frictionX;
    velocityY *= frictionY;

    //Stop tiny velocities
    if (std::fabs(velocityX) < 0.01f) velocityX = 0.f;
    if (std::fabs(velocityY) < 0.01f) velocityY = 0.f;
}

void Ball::Render() const
{
    //On this function we create a circle utilizing the provided DrawLine call, the proccess divides the circle into segments.
    const int segments = 36;
    float angleStep = 2.f * 3.14159f / segments;

    for (int i = 0; i < segments; i++)
    {
        float ax = x + radius * std::cos(i * angleStep);
        float ay = y + radius * std::sin(i * angleStep);
        float bx = x + radius * std::cos((i + 1) * angleStep);
        float by = y + radius * std::sin((i + 1) * angleStep);

        App::DrawLine(ax, ay, bx, by, 1.f, 1.f, 1.f);
    }
}

void Ball::ApplyForce(float forceX, float forceY)
{
    velocityX += forceX;
    velocityY += forceY;
}

float Ball::GetX() const { return x; }
float Ball::GetY() const { return y; }
float Ball::GetRadius() const { return radius; }
