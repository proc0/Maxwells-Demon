#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <raylib.h>
#include <string>

#include "config.h"

// initial screen dimensions
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
// asset files
#define URI_SOUND_SPLAT "splat1.wav"

class Game {
    int count;
    Sound splat;
    
    public:
        int screenWidth = SCREEN_WIDTH;
        int screenHeight = SCREEN_HEIGHT;

        void Load();
        static void Loop(void *self);
        void Render() const;
        void Resize();
        void Run();
        void Unload();
        void Update();
};
