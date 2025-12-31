#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "common.hpp"
#include "entropy.hpp"
#include "chamber.hpp"
#include "gas.hpp"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class Game {
    Gas gas;
    Chamber chamber;
    Entropy entropy;

    public:
        int screenWidth = SCREEN_WIDTH;
        int screenHeight = SCREEN_HEIGHT;

        Game(){};
        ~Game() = default;
        
        void Load();
        static void Loop(void *self);
        void Render(const Thermal) const;
        void Resize();
        void Run();
        Thermal Update();
        void Unload();
};
