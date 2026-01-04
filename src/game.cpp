#include "game.hpp"
#include "raylib.h"

void Game::Load() {
    chamber.Load();
    Thermal stats = gas.Load(chamber);
    State state = {
        .stats = stats
    };
    entropy.Load(state);
    chamber.Init(state);
    display.Load();
}

State Game::Update() {
    // Resize();
    UIEvent event = display.Update();
    if (event.reset) {
        TraceLog(LOG_INFO, "RESET");
    }
    Thermal stats = gas.Update(chamber);
    State state = {
        .stats = stats
    };
    chamber.Update(state);
    entropy.Update(state);

    return state;
}

void Game::Render(const State state) const {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    chamber.Render();
    gas.Render();
    entropy.Render(state);
    display.Render();

    // DrawFPS(20, 20);
    EndDrawing();
}

void Game::Loop(void *self) {
    Game *client = static_cast<Game *>(self);

    State state = client->Update();
    client->Render(state);
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

void Game::Unload() {
    gas.Unload();
    entropy.Unload();
    chamber.Unload();
    display.Unload();
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
