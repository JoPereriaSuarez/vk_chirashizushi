#include "../headers/window_manager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <stdio.h>

uint32_t INIT_FLAGS = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
uint32_t WINDOW_FLAGS = SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN;

static SDL_Window *window = NULL;

int window_create(unsigned int width, unsigned int height)
{
    if(SDL_Init(INIT_FLAGS) < 0)
    {
        printf("Error initializing SDL2\n");
        return -1;
    }

    SDL_CreateWindow("Shirachizushi", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        width, height, WINDOW_FLAGS);
    SDL_SetWindowResizable(window, SDL_TRUE);

    return 0;
}

int get_vk_extensions(unsigned int *count, const char **names)
{
    return 1 - SDL_Vulkan_GetInstanceExtensions(window, count, names);
}

void window_destroy()
{
    SDL_DestroyWindow(window);
    window = NULL;
}