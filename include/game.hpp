#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <raylib.h>

#include "gas.hpp"

// initial screen dimensions
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
// asset files

class Game {
    Gas gas;

    public:
        int screenWidth = SCREEN_WIDTH;
        int screenHeight = SCREEN_HEIGHT;

        Game(){};
        ~Game() = default;
        
        void Load();
        static void Loop(void *self);
        void Render() const;
        void Resize();
        void Run();
        void Unload();
        void Update();
};
