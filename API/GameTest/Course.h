#ifndef COURSE_H
#define COURSE_H

#include <vector>
#include "Ball.h"
#include "Hole.h"
#include "Trampoline.h"
#include "Wall.h"
#include "Spike.h"
#include "EnemyJumper.h"
#include "EnemyShooter.h"

/*
   The Course class controls the collisions between the ball, hole,
   and all the obstacles and enemies. It also controls the level "assets" and the sound effects/background music.
*/

class Course
{
public:
    Course();

    void SetupLevel(int level);
    void Update(float deltaTime);
    void Render() const;

    void ApplyForceToBall(float fx, float fy);

    bool IsLevelComplete() const;

    bool GetWallBounceHappened() const;
    void ResetWallBounceHappened();

    bool GetTrampolineBounceHappened() const;
    void ResetTrampolineBounceHappened();

    //Let the Game pass pointers for scoring & turn logic
    void SetScorePointer(int* scorePtr);
    void SetTurnsPointer(int* turnPtr);

private:
    Ball ball;
    Hole hole;

    std::vector<Trampoline> trampolines;
    std::vector<Wall> walls;
    std::vector<Spike> spikes;
    std::vector<EnemyJumper> jumpers;
    std::vector<EnemyShooter> shooters;

    float spawnX, spawnY;
    bool levelComplete;
    bool wallBounceHappened;
    bool trampolineBounceHappened;

    int* scorePtr;
    int* turnPtr;

    void CheckOutOfBounds();

    //forced = reset score & turns 
    void ForcedRespawn();
    //normal = just place ball 
    void NormalRespawn();

    void PlayBackgroundMusic();
};

#endif 
