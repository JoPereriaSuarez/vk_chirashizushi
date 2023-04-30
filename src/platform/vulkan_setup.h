#define VK_USE_PLATFORM_XLIB_KHR
#include "../logger.h"

#ifndef VULKAN_DEBUG
#define VULKAN_DEBUG
#endif

#ifndef VK_CKECK
#define VK_CHECK_OUT(RESULT, MESSAGE, OUT) ({(*OUT) = RESULT; VK_CHECK(RESULT, MESSAGE);})
#define VK_CHECK(RESULT, MESSAGE) ({int retval = 1; if(RESULT != VK_SUCCESS) {retval = 0; LOG_MESSAGE(LOG_ERROR, MESSAGE);} retval;})
#endif

int init_vulkan();
void destroy_instance();