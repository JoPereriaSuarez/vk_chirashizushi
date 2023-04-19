#define VK_USE_PLATFORM_XLIB_KHR

#ifndef VULKAN_DEBUG
#define VULKAN_DEBUG
#endif

int init_vulkan();
void destroy_instance();