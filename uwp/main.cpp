#include <Windows.h>
#include "SDL.h"
#include "SDL_main.h"

int bootstrap(int argc, char** argv)
{
    SDL_SetHint(SDL_HINT_JOYSTICK_WGI, "true");

    char* local_args[] = {
        "love2d-uwp.exe",
        "E:\\_love\\evilegg.love",
        "\0"
    };

    //return SDL_main(argc, argv);
    return SDL_main(2, local_args);
}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR argv, int argc)
{
    return SDL_RunApp(0, 0, bootstrap, NULL);
}
