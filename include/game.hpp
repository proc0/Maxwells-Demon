#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "common.hpp"
#include "entropy.hpp"
#include "chamber.hpp"
#include "gas.hpp"
#include "display.hpp"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class Game {
    Gas gas;
    Chamber chamber;
    Entropy entropy;
    Display display;

    enum State {
        PLAY,
        PAUSE,
    };
    State state = PLAY;

    public:
        int screenWidth = SCREEN_WIDTH;
        int screenHeight = SCREEN_HEIGHT;

        Game(){};
        ~Game() = default;
        
        void Load(short density);
        static void Loop(void *self);
        void Render(const Memo&) const;
        void Resize();
        void Run();
        Memo Update();
        void Unload();
};
