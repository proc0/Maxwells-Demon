#include "game.hpp"
#include "raylib.h"

void Game::Load(short density) {
    chamber.Load();
    Thermal stats = gas.Load(chamber, density);
    Memo memo = {
        .stats = stats
    };
    entropy.Load(memo);
    chamber.Init(memo);
    display.Load();
}

Memo Game::Update() {
    // Resize();
    UIEvent event = display.Update();
    // if (event.reset) {
    //     Load(25);
    //     return {
    //         .stats = {0},
    //     };
    // }

    if (event.reset) {
        if (state != PAUSE) {
            state = PAUSE;
        } else {
            state = PLAY;
        }
    }

    if (state == PAUSE) {
        return {
            .stats = {0},
        };
    }

    Thermal stats = gas.Update(chamber);
    Memo state = {
        .stats = stats
    };
    chamber.Update(state);
    entropy.Update(state);

    return state;
}

void Game::Render(const Memo& memo) const {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    chamber.Render();
    gas.Render();
    entropy.Render(memo);
    display.Render();

    // DrawFPS(20, 20);
    EndDrawing();
}

void Game::Loop(void *self) {
    Game *client = static_cast<Game *>(self);

    Memo memo = client->Update();
    client->Render(memo);
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
