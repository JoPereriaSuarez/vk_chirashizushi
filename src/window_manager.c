#include "window_manager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <stdio.h>

uint32_t INIT_FLAGS = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
uint32_t WINDOW_FLAGS = SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN;

static SDL_Window *window = NULL;
static SDL_SysWMinfo wm_info;

int window_create(unsigned int width, unsigned int height)
{

    if(SDL_Init(INIT_FLAGS) < 0)
    {
        printf("Error initializing SDL2\n");
        return -1;
    }

    window = SDL_CreateWindow("Shirachizushi", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        width, height, WINDOW_FLAGS);

    SDL_SetWindowResizable(window, SDL_TRUE);

    SDL_VERSION(&wm_info.version);
    printf("[INFO] WM VERSION %d.%d.%d\n", wm_info.version.major, wm_info.version.minor, wm_info.version.patch);

    if(SDL_GetWindowWMInfo(window, &wm_info) == SDL_FALSE)
    {
        printf("[ERROR] Cannot get Window Manager System info\n");
        return -1;
    }

    return 0;
}

int window_get_extensions(unsigned int *count, const char **names)
{
    return 1 - SDL_Vulkan_GetInstanceExtensions(window, count, names);
}

void window_destroy()
{
    SDL_DestroyWindow(window);
    window = NULL;
}

int create_window_surface(VkInstance instance, VkSurfaceKHR *surface)
{
    return 1 - SDL_Vulkan_CreateSurface(window, instance, surface);
}

window_manager_info get_wm_info()
{
    window_manager_info wmi = 
    {
        .xlib_display = wm_info.info.x11.display,
        .xlib_window = wm_info.info.x11.window
    };
    return wmi;
}
