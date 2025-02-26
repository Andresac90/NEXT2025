#ifndef GAME_H
#define GAME_H

#include <memory>
#include "Course.h"

/*
  The Game class controls the three scene transitions: MAIN_MENU, GAMEPLAY, END_SCREEN.
  It also holds global stats (score, best stats) and displays it on screen, the class is also responsable for the mouse logic.
*/

enum class GameState
{
    MAIN_MENU,
    GAMEPLAY,
    END_SCREEN
};

class Game
{
public:
    Game();

    //void Init();
    void Update(float deltaTime);
    void Render() const;

private:
    static const int MAX_LEVELS = 4;

    GameState currentState;
    std::unique_ptr<Course> course;

    //Score & progression
    int playerScore;
    int turnCount;
    int currentLevel;

    //Stats arrays
    int bestScorePerLevel[MAX_LEVELS];
    int lowestTurnsPerLevel[MAX_LEVELS];
    int globalBestScore;
    int globalLowestTurns;

    //Mouse drag-limiter
    bool  isDragging;
    float dragStartX, dragStartY;
    float dragEndX, dragEndY;
    const float MAX_DRAG_DIST = 200.f;

    //Boolean for activating or deactivating the shoot of the ball
    bool canShoot;

    void UpdateMainMenu();
    void RenderMainMenu() const;
    void UpdateGameplay(float deltaTime);
    void RenderGameplay() const;
    void UpdateEndScreen();
    void RenderEndScreen() const;

    //Input & level transitions
    void HandleMouseInput();
    void NextLevel();
    void SaveLevelStats();
};

#endif 
