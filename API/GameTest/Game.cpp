#include "stdafx.h"
#include "Game.h"
#include "app/app.h"
#include <cmath>
#include <cstdio>

Game::Game()
    : currentState(GameState::MAIN_MENU)
    , course(nullptr)
    , playerScore(0)
    , turnCount(0)
    , currentLevel(1)
    , isDragging(false)
    , dragStartX(0.f), dragStartY(0.f)
    , dragEndX(0.f), dragEndY(0.f)
    , canShoot(true)
    , globalBestScore(0)
    , globalLowestTurns(0)
{
    for (int i = 0; i < MAX_LEVELS; i++)
    {
        bestScorePerLevel[i] = 0;
        lowestTurnsPerLevel[i] = 0;
    }
}

//void Game::Init()
//{
//    
//}

void Game::Update(float deltaTime)
{
    switch (currentState)
    {
    case GameState::MAIN_MENU:
        UpdateMainMenu();
        break;
    case GameState::GAMEPLAY:
        UpdateGameplay(deltaTime);
        break;
    case GameState::END_SCREEN:
        UpdateEndScreen();
        break;
    }
}

void Game::Render() const
{
    switch (currentState)
    {
    case GameState::MAIN_MENU:
        RenderMainMenu();
        break;
    case GameState::GAMEPLAY:
        RenderGameplay();
        break;
    case GameState::END_SCREEN:
        RenderEndScreen();
        break;
    }
}

//MAIN MENU
void Game::UpdateMainMenu()
{
    if (App::IsKeyPressed(VK_RETURN))
    {
        currentState = GameState::GAMEPLAY;
        playerScore = 0;
        turnCount = 0;
        currentLevel = 1;

        course = std::make_unique<Course>();
        course->SetupLevel(currentLevel);
        course->SetScorePointer(&playerScore);
        course->SetTurnsPointer(&turnCount);

        canShoot = true;
    }
}

void Game::RenderMainMenu() const
{
    App::Print(445, 450, "TRICKSHOT MINIGOLF", 1.f, 1.f, 0.f);
    App::Print(457, 400, "Press Enter to Start!", 1.f, 1.f, 1.f);
}

//GAMEPLAY
void Game::UpdateGameplay(float deltaTime)
{
    if (!course)
    {
        course = std::make_unique<Course>();
        course->SetupLevel(currentLevel);
        course->SetScorePointer(&playerScore);
        course->SetTurnsPointer(&turnCount);
        canShoot = true;
    }

    HandleMouseInput();
    course->Update(deltaTime);

    //If the ball hits a wall or trampoline the boolean to shoot turns true
    if (course->GetWallBounceHappened() || course->GetTrampolineBounceHappened())
    {
        canShoot = true;
        course->ResetWallBounceHappened();
        course->ResetTrampolineBounceHappened();
    }

    if (course->IsLevelComplete())
    {
        //Add 300 to the Score only if the player make a hole in one, the normal reward for completing the level is 50
        if (turnCount == 1)
            playerScore += 300;
        else
            playerScore += 50;

        SaveLevelStats();
        NextLevel();
    }
}

void Game::RenderGameplay() const
{
    if (course)
    {
        course->Render();
    }

    if (isDragging)
    {
        App::DrawLine(dragStartX, dragStartY, dragEndX, dragEndY, 1.f, 0.f, 0.f);
    }

    //Text to show controls
    App::Print(270.f, 740.f, "Drag Mouse: Hit Ball", 1.f, 1.f, 1.f);
    App::Print(650.f, 740.f, "R: Reset Level", 1.f, 1.f, 1.f);

    //Text to know if the player can drag the ball, and change color
    if (canShoot)
    {
        App::Print(480.f, 740.f, "Hit the ball!", 0.f, 255.f, 0.f);
    }
    else
    {
        App::Print(440.f, 740.f, "Wait to hit the ball again!", 255.f, 0.f, 0.f);
    }

    //Local stats
    char buffer[128];
    std::sprintf(buffer, "Score: %d", playerScore);
    App::Print(10.f, 740.f, buffer, 0.f, 0.f, 255.f);

    std::sprintf(buffer, "Turns: %d", turnCount);
    App::Print(10.f, 705.f, buffer, 0.f, 0.f, 255.f);

    std::sprintf(buffer, "Level: %d", currentLevel);
    App::Print(10.f, 670.f, buffer, 0.f, 0.f, 255.f);

    if (currentLevel >= 1 && currentLevel <= MAX_LEVELS)
    {
        int idx = currentLevel - 1;
        char statBuf[128];
        std::sprintf(statBuf, "Best Score L%d: %d", currentLevel, bestScorePerLevel[idx]);
        App::Print(900.f, 740.f, statBuf, 1.f, 1.f, 0.f);

        std::sprintf(statBuf, "Lowest Turns L%d: %d", currentLevel, lowestTurnsPerLevel[idx]);
        App::Print(900.f, 705.f, statBuf, 1.f, 1.f, 0.f);
    }
}

//END SCREEN
void Game::UpdateEndScreen()
{
    float mx, my;
    App::GetMousePos(mx, my);

    if (App::IsKeyPressed(VK_LBUTTON))
    {
        if (mx >= 400 && mx <= 600 && my >= 200 && my <= 320)
        {
            App::StopSound(".\\TestData\\BackgroundMusicEdited.mp3");
            currentState = GameState::MAIN_MENU;
        }
    }
}

void Game::RenderEndScreen() const
{
    App::Print(450, 600, "Thank you for playing!", 0.f, 128.f, 0.f);

    int yPos = 550;
    for (int i = 0; i < MAX_LEVELS; i++)
    {
        char buf[128];
        std::sprintf(buf, "Level %d: Best Score: %d, Lowest Turns: %d",
            i + 1, bestScorePerLevel[i], lowestTurnsPerLevel[i]);
        App::Print(390, yPos, buf, 1.f, 1.f, 1.f);
        yPos -= 30;
    }

    char globalBuf[128];
    std::sprintf(globalBuf, "Global Best Score: %d, Global Lowest Turns: %d",
        globalBestScore, globalLowestTurns);
    App::Print(390, yPos - 30, globalBuf, 1.f, 1.f, 1.f);

    App::Print(450, 250, "Back to Main Menu", 1.f, 1.f, 0.f);
}

//INPUT
void Game::HandleMouseInput()
{
    if (!canShoot)
        return;

    float mx, my;
    App::GetMousePos(mx, my);

    if (App::IsKeyPressed(VK_LBUTTON))
    {
        if (!isDragging)
        {
            isDragging = true;
            App::PlaySound(".\\TestData\\DragBall.mp3");
            dragStartX = mx;
            dragStartY = my;
        }
        dragEndX = mx;
        dragEndY = my;

        //Clamp distance
        float dx = dragEndX - dragStartX;
        float dy = dragEndY - dragStartY;
        float dist = std::sqrt(dx * dx + dy * dy);
        if (dist > MAX_DRAG_DIST)
        {
            float angle = std::atan2(dy, dx);
            dragEndX = dragStartX + std::cos(angle) * MAX_DRAG_DIST;
            dragEndY = dragStartY + std::sin(angle) * MAX_DRAG_DIST;
        }
    }
    else if (isDragging)
    {
        //The drag is released
        isDragging = false;
        App::PlaySound(".\\TestData\\HitBall.mp3");
        turnCount++;

        float dx = dragEndX - dragStartX;
        float dy = dragEndY - dragStartY;
        float forceScale = 2.1f;
        float fx = dx * forceScale;
        float fy = dy * forceScale;

        if (course)
        {
            course->ApplyForceToBall(-fx, -fy);
        }

        //Wait for a bounce to reactivate the shoot function
        canShoot = false;
    }
}

//LEVEL FLOW
void Game::NextLevel()
{
    currentLevel++;
    turnCount = 0;
    playerScore = 0;

    if (currentLevel > MAX_LEVELS)
    {
        currentState = GameState::END_SCREEN;
        return;
    }

    if (course)
    {
        course->SetupLevel(currentLevel);
    }
    canShoot = true;
}

void Game::SaveLevelStats()
{
    int idx = currentLevel - 1;
    if (playerScore > bestScorePerLevel[idx])
    {
        bestScorePerLevel[idx] = playerScore;
    }
    if (lowestTurnsPerLevel[idx] == 0 || turnCount < lowestTurnsPerLevel[idx])
    {
        lowestTurnsPerLevel[idx] = turnCount;
    }
    if (playerScore > globalBestScore)
    {
        globalBestScore = playerScore;
    }
    if (globalLowestTurns == 0 || turnCount < globalLowestTurns)
    {
        globalLowestTurns = turnCount;
    }
}
