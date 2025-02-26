#include "stdafx.h"
#include "Course.h"
#include "app/app.h"

Course::Course()
    : spawnX(100.f)
    , spawnY(100.f)
    , levelComplete(false)
    , wallBounceHappened(false)
    , trampolineBounceHappened(false)
    , scorePtr(nullptr)
    , turnPtr(nullptr)
{
}

void Course::SetupLevel(int level)
{
    PlayBackgroundMusic();

    trampolines.clear();
    walls.clear();
    spikes.clear();
    jumpers.clear();
    shooters.clear();

    levelComplete = false;
    wallBounceHappened = false;
    trampolineBounceHappened = false;

    spawnX = 100.f;
    spawnY = 100.f;
    ball.SetPosition(spawnX, spawnY);
    ball.SetVelocity(0.f, 0.f);

    if (level == 1)
    {
        //Default spawn points for this level 
        spawnX = 235.f;
        spawnY = 230.f;

        //Ball default position
        ball.SetPosition(235, 230);

        //First Trampoline
        trampolines.emplace_back(200.f, 80.f, 150.f, 1.5f);
        trampolines.back().Init();

        //Second trampoline
        trampolines.emplace_back(400.f, 285.f, 150.f, 1.5f);
        trampolines.back().Init();

        //Second trampoline
        trampolines.emplace_back(600.f, 420.f, 150.f, 1.5f);
        trampolines.back().Init();

        // Walls surrounding the level
        walls.emplace_back(1.f, 0.f, 1.f, 650.f);// left
        walls.emplace_back(1020.f, 0.f, 1020.f, 650.f);// right
        walls.emplace_back(0.f, 650.f, 1020.f, 650.f);// top

        //Hole
        hole = Hole(750.f, 620.f, 15.f);
    }
    else if (level == 2)
    {
        //Default spawn points for this level 
        spawnX = 250.f;
        spawnY = 95.f;

        //Ball default position
        ball.SetPosition(250, 95);

        //Ball wall spawnpoint
        walls.emplace_back(180.f, 80.f, 280.f, 80.f);

        //Jumper
        jumpers.emplace_back(350.f, 125.f, 100.f, 20.f);

        // First trampoline
        trampolines.emplace_back(450.f, 285.f, 150.f, 1.5f, 70.f);
        trampolines.back().Init();

        // Walls surrounding the level
        walls.emplace_back(1.f, 0.f, 1.f, 650.f);// left
        walls.emplace_back(1020.f, 0.f, 1020.f, 650.f);// right
        walls.emplace_back(0.f, 650.f, 1020.f, 650.f);// top

        //Hole
        hole = Hole(465.f, 620.f, 15.f);
    }
    else if (level == 3)
    {
        //Default spawn points for this level 
        spawnX = 250.f;
        spawnY = 295.f;

        //Ball default position
        ball.SetPosition(250, 295);

        walls.emplace_back(0.f, 260.f, 1200.f, 260.f);   // bottom
        walls.emplace_back(0.f, 0.f, 0.f, 600.f); // left
        walls.emplace_back(1200.f, 0.f, 1200.f, 600.f); // right
        walls.emplace_back(0.f, 500.f, 1200.f, 500.f); // top

        //Some spikes
        spikes.emplace_back(400.f, 450.f, 15.f);
        spikes.emplace_back(500.f, 380.f, 15.f);
        spikes.emplace_back(600.f, 450.f, 15.f);

        //Shooter with low firerate
        shooters.emplace_back(950.f, 450.f, 1.1f, 125.f, 15.f);

        hole = Hole(500.f, 450.f, 15.f);
    }
    else if (level == 4)
    {
        //Default spawn points for this level 
        spawnX = 500.f;
        spawnY = 590.f;

        //Ball default position
        ball.SetPosition(500, 590);

        //Ball wall spawnpoint
        walls.emplace_back(470.f, 575.f, 530.f, 575.f);

        //Wall Below spawnpoint
        walls.emplace_back(470.f, 440.f, 530.f, 440.f);

        //Left Trampoline with less intensity
        trampolines.emplace_back(230.f, 460.f, 60.f, 0.95f);
        trampolines.back().Init();

        //Low Left trampoline 
        trampolines.emplace_back(385.f, 245.f, 70.f, 0.95f);
        trampolines.back().Init();
        //Shooter with high firerate
        shooters.emplace_back(300.f, 265.f, 0.85f, 160.f, 15.f);

        //Right Trampoline with less intensity
        trampolines.emplace_back(800.f, 460.f, 60.f, 0.95f);
        trampolines.back().Init();

        //Low Right trampoline 
        trampolines.emplace_back(645.f, 245.f, 70.f, 0.95f);
        trampolines.back().Init();
        //Jumper with higer jump height
        jumpers.emplace_back(730.f, 190.f, 250.f, 20.f);

        // Walls surrounding the level
        walls.emplace_back(1.f, 0.f, 1.f, 650.f);// left
        walls.emplace_back(1020.f, 0.f, 1020.f, 650.f);// right
        walls.emplace_back(0.f, 650.f, 1020.f, 650.f);// top

        //Hole
        hole = Hole(500.f, 150.f, 15.f);

        //Spikes
        spikes.emplace_back(570.f, 190.f, 15.f);
        spikes.emplace_back(440.f, 190.f, 15.f);
    }
    else
    {
        //If the level is outside 1..4 for some bug, it defaults to this
        hole = Hole(800.f, 300.f, 15.f);
    }
}

void Course::PlayBackgroundMusic()
{
    if (!App::IsSoundPlaying(".\\TestData\\BackgroundMusicEdited.mp3"))
    {
        App::PlaySound(".\\TestData\\BackgroundMusicEdited.mp3", true);
    }
}

void Course::Update(float deltaTime)
{
    //Optional forced respawn if 'R' pressed
    if (App::IsKeyPressed('R'))
    {
        ForcedRespawn();
    }

    ball.Update(deltaTime);

    //TRAMPOLINES
    for (auto& t : trampolines)
    {
        //We reset the 'didReEnableShooting' each loop to detect new bounces
        t.Update(deltaTime);

        bool newBounce = t.HandleCollision(ball);
        if (newBounce)
        {
            //The player gains +200 score only once per bounce
            if (scorePtr)
                (*scorePtr) += 200;
        }

        //Enable shooting the ball again when a bounce in a trampoline is detected
        if (t.DidReEnableShootingOnBounce())
        {
            trampolineBounceHappened = true;
            t.ResetReEnableShootingFlag();
        }
    }

    //WALLS
    wallBounceHappened = false;
    for (auto& w : walls)
    {
        bool bounced = w.HandleCollision(ball);
        //Enable shooting the ball again when a bounce in a wall is detected
        if (bounced)
        {
            wallBounceHappened = true;
        }
    }

    //SPIKES
    for (auto& s : spikes)
    {
        if (s.CheckCollision(ball))
        {
            App::PlaySound(".\\TestData\\Spikes.mp3");
            //The player losses 100 of score when it crahes with a spike
            if (scorePtr)
                (*scorePtr) -= 100;
            NormalRespawn();
        }
    }

    //JUMPER ENEMIES
    for (auto& j : jumpers)
    {
        j.Update(deltaTime);
        if (j.CheckCollision(ball))
        {
            App::PlaySound(".\\TestData\\HitEnemy.mp3");
            //The player losses 75 of score when it crahes with a jumper enemy
            if (scorePtr)
                (*scorePtr) -= 75;
            NormalRespawn();
        }
    }

    //SHOOTER ENEMIES
    for (auto& sh : shooters)
    {
        sh.Update(deltaTime, ball);
        if (sh.CheckProjectilesCollision(ball))
        {
            App::PlaySound(".\\TestData\\HitBullet.mp3");
            //The player losses 25 of score when it crahes with a bullet of a shooter enemy
            if (scorePtr)
                (*scorePtr) -= 25;
            NormalRespawn();
        }
    }

    //HOLE, Check if level is completed
    if (hole.CheckCollision(ball))
    {
        levelComplete = true;
        App::PlaySound(".\\TestData\\NextLevel.mp3");
    }

    CheckOutOfBounds();
}

void Course::Render() const
{
    ball.Render();
    hole.Render();

    for (auto& t : trampolines) { t.Render(); }
    for (auto& w : walls) { w.Render(); }
    for (auto& s : spikes) { s.Render(); }
    for (auto& j : jumpers) { j.Render(); }
    for (auto& sh : shooters) { sh.Render(); }
}

void Course::ApplyForceToBall(float fx, float fy)
{
    ball.ApplyForce(fx, fy);
}

bool Course::IsLevelComplete() const
{
    return levelComplete;
}

bool Course::GetWallBounceHappened() const
{
    return wallBounceHappened;
}
void Course::ResetWallBounceHappened()
{
    wallBounceHappened = false;
}

bool Course::GetTrampolineBounceHappened() const
{
    return trampolineBounceHappened;
}
void Course::ResetTrampolineBounceHappened()
{
    trampolineBounceHappened = false;
}

void Course::SetScorePointer(int* sPtr)
{
    scorePtr = sPtr;
}
void Course::SetTurnsPointer(int* tPtr)
{
    turnPtr = tPtr;
}

//Forced Respawn, resets everything, including the current scoreboard
void Course::ForcedRespawn()
{
    ball.SetPosition(spawnX, spawnY);
    ball.SetVelocity(0.f, 0.f);

    if (scorePtr) (*scorePtr) = 0;
    if (turnPtr)  (*turnPtr) = 0;

    for (auto& t : trampolines)
    {
        t.ResetBounceThisLevel();
    }
}

//Normal Respawn, resets only the ball to its spawn position
void Course::NormalRespawn()
{
    ball.SetPosition(spawnX, spawnY);
    ball.SetVelocity(0.f, 0.f);
}

//Check if ball is out-of-bounds to call normal respawn
void Course::CheckOutOfBounds()
{
    float bx = ball.GetX();
    float by = ball.GetY();
    if (bx < -100.f || bx > 2000.f || by < -100.f || by > 1500.f)
    {
        NormalRespawn();
    }
}
