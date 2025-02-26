#include "stdafx.h"
#include "Game.h"

static Game* game = nullptr;
//GameTest access all the functions of the Game class that contains the main logic.
void Init()
{
    game = new Game();
    //game->Init();
}

void Update(float deltaTime)
{
    if (game)
    {
        game->Update(deltaTime);
    }
}

void Render()
{
    if (game)
    {
        game->Render();
    }
}

void Shutdown()
{
    if (game)
    {
        delete game;
        game = nullptr;
    }
}
