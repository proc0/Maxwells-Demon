#include "game.hpp"

void Game::Load(short density) {
    chamber.Load();
    Thermal stats = gas.Load(chamber, density);
    Memo memo = { .stats = stats };
    cache = memo;
    entropy.Load(memo);
    chamber.Init(memo);
    demon.Load();
    display.Load();
    maxwell->Load();
}

void Game::Init(short density) {
    chamber.Load();
    Thermal stats = gas.Load(chamber, density);
    Memo memo = { .stats = stats };
    cache = memo;
    entropy.Init(memo);
    chamber.Init(memo);
    maxwell->Init();
    isToasted = false;
}

Memo Game::Update() {
    // Resize();
    UIEvent event = display.Update();

    if (event.reset) {
        Init(DENSITY_START);
        return cache;
    }

    if (event.pause) {
        if (state == State::PLAY) {
            state = State::PAUSE;
            Thermal stats = gas.Update(chamber);
            Memo memo = { .stats = stats };
            chamber.Update(memo);
            entropy.Update(memo);
            cache = memo;
            return memo;
        } else {
            state = State::PLAY;
        }
    }

    if (event.add) {
        gas.Add(chamber);
        Thermal stats = gas.Count();
        Memo memo = { .stats = stats };
        chamber.Init(memo);
        chamber.UpdateColors(memo.stats);
        entropy.Init(memo);
        cache = memo;
        return memo;
    }

    if (event.sub) {
        gas.Sub(chamber);
        Thermal stats = gas.Count();
        Memo memo = { .stats = stats };
        chamber.Init(memo);
        chamber.UpdateColors(memo.stats);
        entropy.Init(memo);
        cache = memo;
        return memo;
    }

    if (state == State::PAUSE) {
        return cache;
    }

    Thermal stats = gas.Update(chamber);
    Memo memo = { .stats = stats };
    chamber.Update(memo);
    demon.Update();

    bool isZero = entropy.Update(memo);
    if(isZero && !isToasted) {
        maxwell->Toasty();
        isToasted = true;
    }

    if (isToasted) {
        maxwell->Update();
    }

    return memo;
}

void Game::Render(const Memo& memo) const {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    chamber.Render();
    gas.Render();
    entropy.Render(memo);
    display.Render();
    demon.Render();
    maxwell->Render();
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
    maxwell->Unload();
    delete maxwell;
    display.Unload();
    demon.Unload();
    entropy.Unload();
    chamber.Unload();
    gas.Unload();
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
