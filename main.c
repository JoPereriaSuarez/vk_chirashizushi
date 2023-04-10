#include <stdio.h>
#include "headers/renderer_vulkan.h"
#include "headers/window_manager.h"
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

__uint8_t quit = 0;

void cleanup()
{
    window_destroy();
    destroy_instance();
}

void main_loop()
{
    while(quit == 0)
    {
        SDL_Event event_ptr;
        while(SDL_PollEvent(&event_ptr))
        {
            switch (event_ptr.type)
            {
                case SDL_QUIT:
                    quit = 1;
                    break;
                default:
                    break;
            }
        }
    }

    if(quit == 1)
    {
        cleanup();
    }
}

void run()
{
    if(window_create(WINDOW_WIDTH, WINDOW_HEIGHT) != 0)
    {
        cleanup();
        return;
    }
    if(init_vulkan() != 0)
    {
        cleanup();
        return;
    }

    main_loop();
}

int main(char argc, char *argv[])
{
    run();
}