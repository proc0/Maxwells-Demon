#include "game.hpp"

#if __EMSCRIPTEN__
EM_JS(int, getBrowserWidth, (), {
    return window.innerWidth;
});

EM_JS(int, getBrowserHeight, (), {
    return window.innerHeight;
});
#endif

void Game::Resize()
{
    int height = GetScreenHeight();
    int width = GetScreenWidth();

    if (screenHeight != height || screenWidth != width)
    {
        screenHeight = height;
        screenWidth = width;

#if __EMSCRIPTEN__
        static int PADDING = 30; // set padding to avoid scrollbar and browser edge overlap
        SetWindowSize(getBrowserWidth() - PADDING, getBrowserHeight() - PADDING);
#endif
    }
}

void Game::Update()
{

    // Resize();
    chamber.Update();
    gas.Update(chamber);
    // TODO: find another way to share data between gas and chamber, maybe a struct with these numbers
    chamber.UpdateColors(gas.leftChamberCoolCount, gas.leftChamberHotCount, gas.rightChamberCoolCount, gas.rightChamberHotCount, gas.totalCoolCount, gas.totalHotCount);
}

void Game::Render() const
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    chamber.Render();
    gas.Render();

    // DrawFPS(20, 20);
    EndDrawing();
}

void Game::Load()
{
    chamber.Load();
    gas.Load(chamber);
    gas.screenWidth = screenWidth;
    gas.screenHeight = screenHeight;
}

void Game::Unload()
{
    gas.Unload();
    chamber.Unload();
}

void Game::Loop(void *self)
{
    Game *client = static_cast<Game *>(self);

    client->Update();
    client->Render();
}

void Game::Run()
{
#ifdef __EMSCRIPTEN__
    // no target FPS for performance
    emscripten_set_main_loop_arg(Loop, this, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        Loop(this);
    }
#endif
}
