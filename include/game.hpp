#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "common.hpp"
#include "entropy.hpp"
#include "chamber.hpp"
#include "gas.hpp"
#include "demon.hpp"
#include "display.hpp"
#include "maxwell.hpp"

class Game {
    Gas gas;
    Chamber chamber;
    Display display;
    Entropy entropy;
    Demon demon;
    Memo cache = { .stats = {0} };
    Maxwell* maxwell = new Maxwell();

    enum State {
        PLAY,
        PAUSE,
    };
    State state = State::PLAY;
    bool isToasted = false;

    public:
        int screenWidth = SCREEN_WIDTH;
        int screenHeight = SCREEN_HEIGHT;

        Game(){};
        ~Game() = default;
        
        void Load(short density);
        void Init(short density);
        static void Loop(void *self);
        void Render(const Memo&) const;
        void Resize();
        void Run();
        Memo Update();
        void Unload();
};
