#include <Windows.h>
#include "SDL2/SDL.h"

int bootstrap(int argc, char** argv)
{
    char* local_args[] = {
        "love2d-uwp.exe",
        "E:\\_love\\revolver.love",
        "\0"
    };

    //return SDL_main(argc, argv);
    return SDL_main(2, local_args);
}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR argv, int argc)
{
    return SDL_WinRTRunApp(bootstrap, NULL);
}
