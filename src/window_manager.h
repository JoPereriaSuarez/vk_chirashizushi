#include <SDL2/SDL_syswm.h>
#include <string.h>

#define SESSION_TYPE getenv("XDG_SESSION_TYPE")
#define USING_X11() strcmp(SESSION_TYPE, "x11")
#define USING_WAYLAND() strcmp(SESSION_TYPE, "wayland")

typedef struct window_manager_info
{
#ifdef SDL_VIDEO_DRIVER_WINDOWS
    void *window;
    void *hdc;
    void *h_instance;
#endif

#ifdef SDL_VIDEO_DRIVER_WAYLAND
    void *wl_display;
    void *wl_surface;
    void *shell_surface;
    void *wl_egl_window;
    void *xdg_surface;
    void *xdg_top_level;
#endif

#ifdef SDL_VIDEO_DRIVER_X11
    Display *xlib_display;
    Window xlib_window;
#endif

}window_manager_info;

int window_create(unsigned int width, unsigned int height);
int window_get_extensions(unsigned int *count, const char **names);
void window_destroy();
window_manager_info get_wm_info();
