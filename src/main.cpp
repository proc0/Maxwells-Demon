#include "game.hpp"
#include "config.h"

int main(void){
    Game client;

#ifdef __EMSCRIPTEN__
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
#else
    // SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
#endif

    InitWindow(client.screenWidth, client.screenHeight, PROJECT_NAME);
    InitAudioDevice();
    
    client.Load();
    client.Run();
    client.Unload();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
