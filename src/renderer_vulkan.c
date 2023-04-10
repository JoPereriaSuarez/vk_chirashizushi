#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>
#include "../headers/renderer_vulkan.h"
#include "../headers/window_manager.h"

static VkInstance instance = NULL;

int init_vulkan()
{
    VkApplicationInfo app_info = 
    {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = "Shirachizushi",
        .applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 1),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0),
        .apiVersion = VK_API_VERSION_1_3
    };   

    unsigned int extension_count = 0;
    const char **extensions = malloc(sizeof(char *) * extension_count);
    if(get_vk_extensions(&extension_count, NULL) != 0)
    {
        printf("Error creating extensions count\n");
        return 1;
    }
    if(get_vk_extensions(&extension_count, extensions) != 0)
    {
        printf("Error creating extensions\n");
        return 1;
    }

    VkInstanceCreateInfo instance_info =
    {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_info,
        .enabledExtensionCount = extension_count,
        .ppEnabledExtensionNames = extensions
    };
    VkResult instance_result = vkCreateInstance(&instance_info, NULL, &instance);
    if(instance_result != VK_SUCCESS)
    {
        printf("Error creating vulkan instance code: %d", instance_result);
        return 1;
    }
}

void destroy_instance()
{
    vkDestroyInstance(instance, NULL);
    instance = NULL;
}