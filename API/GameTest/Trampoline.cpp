#include "stdafx.h"
#include "Trampoline.h"
#include "app/app.h"
#include <cmath>
#include <vector>

// Helper function to convert degrees to radians
static float Deg2Rad(float deg)
{
    return deg * 3.14159f / 180.f;
}

//Constructor that initializes trampoline properties
Trampoline::Trampoline(float x, float y, float length, float intensity, float orientationDeg)
    : x(x)
    , y(y)
    , length(length)
    , intensity(intensity)
    , orientationDeg(orientationDeg)
    , springConstant(0.02f)     //Determines the stiffness of the springs
    , damping(0.04f)            //Controls the energy loss over time
    , propagationSpeed(0.25f)   //Speed at which waves travel along the trampoline
    , hasBouncedThisLevel(false)
    , reEnableShooting(false)
{
}

//Initializes the trampoline by creating spring points
void Trampoline::Init()
{
    int numPoints = static_cast<int>(length / 8.0f);
    if (numPoints < 2)
        numPoints = 2;

    float pointSpacing = length / (numPoints - 1);
    points.clear();
    points.reserve(numPoints);

    float angleRad = Deg2Rad(orientationDeg);
    float cosA = cosf(angleRad);
    float sinA = sinf(angleRad);

    //Create spring points in local coordinates and rotate them based on orientation
    for (int i = 0; i < numPoints; i++)
    {
        float localX = -length * 0.5f + i * pointSpacing;
        float localY = 0.f;

        //Rotate local coordinates to global coordinates
        float rx = localX * cosA - localY * sinA;
        float ry = localX * sinA + localY * cosA;

        SpringPoint sp;
        sp.x = x + rx;
        sp.y = y + ry;
        sp.restY = sp.y;
        sp.velocity = 0.f;
        points.push_back(sp);
    }
}

//In this Update function, the trampoline's state is updated based on the physics calculations
void Trampoline::Update(float deltaTime)
{
    static std::vector<float> accelerations;
    accelerations.resize(points.size(), 0.f);

    //Calculate accelerations based on Hooke's Law and damping
    for (size_t i = 0; i < points.size(); i++)
    {
        float stretch = points[i].y - points[i].restY;
        float a = -springConstant * stretch - damping * points[i].velocity;
        accelerations[i] = a;
    }

    //Integrate velocities and positions
    for (size_t i = 0; i < points.size(); i++)
    {
        points[i].velocity += accelerations[i];
        points[i].y += points[i].velocity;
    }

    //Propagate wave effects to neighboring points
    std::vector<float> leftD(points.size(), 0.f);
    std::vector<float> rightD(points.size(), 0.f);

    for (size_t i = 0; i < points.size(); i++)
    {
        if (i > 0)
        {
            float delta = points[i].y - points[i - 1].y;
            leftD[i] = delta * propagationSpeed;
        }
        if (i < points.size() - 1)
        {
            float delta = points[i].y - points[i + 1].y;
            rightD[i] = delta * propagationSpeed;
        }
    }

    //Apply wave propagation deltas to neighboring points
    for (size_t i = 0; i < points.size(); i++)
    {
        if (i > 0)
        {
            points[i - 1].velocity += leftD[i];
        }
        if (i < points.size() - 1)
        {
            points[i + 1].velocity += rightD[i];
        }
    }
}

void Trampoline::Render() const
{
    if (points.size() < 2)
        return;

    //Simple green line to represent the trampoline
    for (size_t i = 0; i < points.size() - 1; i++)
    {
        App::DrawLine(points[i].x, points[i].y,
            points[i + 1].x, points[i + 1].y,
            0.f, 1.f, 0.f);
    }
}

bool Trampoline::HandleCollision(Ball& ball)
{
    //Reset reEnableShooting for this frame
    reEnableShooting = false;

    if (points.empty())
        return false;

    float bx = ball.GetX();
    float by = ball.GetY();
    float br = ball.GetRadius();

    //Bounding box for quick collision check
    float minX = points[0].x, maxX = points[0].x;
    float minY = points[0].y, maxY = points[0].y;
    for (auto& p : points)
    {
        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;
    }

    if ((bx + br) < minX || (bx - br) > maxX ||
        (by + br) < minY || (by - br) > maxY)
    {
        return false;
    }

    //Check for Ball's bottom edge vs trampoline's top
    if ((by - br) <= maxY)
    {
        float overlap = (maxY+13) - (by - br); //13 is for give an extra Y space so that the ball doesnt passes through the trampoline
        if (overlap < 0.f)
            return false;

        float bounceForce = overlap * intensity;
        if (bounceForce > 100.f)
            bounceForce = 100.f;

        ball.ApplyForce(0.f, bounceForce);

        //Find the closest spring point and generate a ripple
        float bestDistSq = 9999999.f;
        int bestIdx = -1;
        for (int i = 0; i < (int)points.size(); i++)
        {
            float dx = bx - points[i].x;
            float dy = (by - br) - points[i].y;
            float distSq = dx * dx + dy * dy;
            if (distSq < bestDistSq)
            {
                bestDistSq = distSq;
                bestIdx = i;
            }
        }
        if (bestIdx >= 0)
        {
            GenerateRipple(bestIdx, bounceForce);
        }

        //Re-enable shooting the ball after a bounce
        reEnableShooting = true;

        //The first bounce per level awards 250 of Score
        if (!hasBouncedThisLevel)
        {
            hasBouncedThisLevel = true;
            return true;
        }
    }
    return false;
}

//Applies a velocity disturbance to the specified spring point and simulates the visual effect of the trampoline reacting to the bounce
void Trampoline::GenerateRipple(int pointIndex, float force)
{
    if (pointIndex >= 0 && pointIndex < (int)points.size())
    {
        //Add trampoline sound effect
        App::PlaySound(".\\TestData\\Trampoline.mp3");

        points[pointIndex].velocity -= force * 0.1f;
    }
}
