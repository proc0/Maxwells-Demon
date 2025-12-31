#include "game.hpp"


Thermal Game::Update()
{
    // Resize();
    Thermal stats = gas.Update(chamber);
    chamber.Update(stats);
    entropy.Update(stats);

    return stats;
}

void Game::Render(const Thermal stats) const {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    chamber.Render();
    gas.Render();
    entropy.Render(stats);

    // DrawFPS(20, 20);
    EndDrawing();
}

void Game::Load() {
    chamber.Load();
    Thermal stats = gas.Load(chamber);
    entropy.Load(stats);
    chamber.Init(stats);
}

void Game::Unload() {
    gas.Unload();
    chamber.Unload();
}

void Game::Loop(void *self) {
    Game *client = static_cast<Game *>(self);

    Thermal stats = client->Update();
    client->Render(stats);
}

void Game::Run() {
#ifdef __EMSCRIPTEN__
    // no target FPS for browser performance
    emscripten_set_main_loop_arg(Loop, this, 0, 1);
#else
    SetTargetFPS(120);
    while (!WindowShouldClose())
    {
        Loop(this);
    }
#endif
}

// #if __EMSCRIPTEN__
// EM_JS(int, getBrowserWidth, (), {
//     return window.innerWidth;
// });

// EM_JS(int, getBrowserHeight, (), {
//     return window.innerHeight;
// });
// #endif

// void Game::Resize()
// {
//     int height = GetScreenHeight();
//     int width = GetScreenWidth();

//     if (screenHeight != height || screenWidth != width)
//     {
//         screenHeight = height;
//         screenWidth = width;

// #if __EMSCRIPTEN__
//         static int PADDING = 30; // set padding to avoid scrollbar and browser edge overlap
//         SetWindowSize(getBrowserWidth() - PADDING, getBrowserHeight() - PADDING);
// #endif
//     }
// }
