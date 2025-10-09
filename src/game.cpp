#include "game.hpp"

#if __EMSCRIPTEN__
EM_JS(int, getBrowserWidth, (), {
    return window.innerWidth;
});

EM_JS(int, getBrowserHeight, (), {
    return window.innerHeight;
});
#endif

void Game::Resize() {
    int height = GetScreenHeight();
    int width = GetScreenWidth();

    if(screenHeight != height || screenWidth != width){
        screenHeight = height;
        screenWidth = width;

    #if __EMSCRIPTEN__
        static int PADDING = 30; // set padding to avoid scrollbar and browser edge overlap
        SetWindowSize(getBrowserWidth() - PADDING, getBrowserHeight() - PADDING);
    #endif
    }
}

void Game::Update(){

    Resize();

    if(IsKeyPressed(KEY_SPACE)){
        count++;
        PlaySound(splat);
    }
    
    if (IsKeyPressed(KEY_H)){
        if (IsCursorHidden())
        {
            ShowCursor();
        }
        else
        {
            HideCursor();
        }
    }
    
}

void Game::Render() const {
    BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleGradientH(0, 0, screenWidth, screenHeight, BLUE, ORANGE);
        
        DrawCircleV(GetMousePosition(), 40, YELLOW);

        const char* countText = TextFormat("Count: %i", count);
        DrawText(countText, 50, 50, 20, WHITE);
        if (IsCursorHidden()) {
            DrawText("CURSOR HIDDEN", screenWidth/2-100, 60, 20, RED);
        } else {
            DrawText("CURSOR VISIBLE", screenWidth/2-100, 60, 20, GREEN);
        }
    EndDrawing();
}

void Game::Load(){
    std::string pathAssets = DIR_ASSETS;
    const char* pathSoundSplat = pathAssets.append("/").append(URI_SOUND_SPLAT).c_str();

    splat = LoadSound(pathSoundSplat);
    count = 0;
}

void Game::Unload(){
    UnloadSound(splat);
}

void Game::Loop(void *self) {
    Game *client = static_cast<Game *>(self);

    client->Update();
    client->Render();
}

void Game::Run() {
    #ifdef __EMSCRIPTEN__
        // no target FPS for performance
        emscripten_set_main_loop_arg(Loop, this, 0, 1);
    #else
        SetTargetFPS(60);
        while (!WindowShouldClose()) {
            Loop(this);
        }
    #endif
}
