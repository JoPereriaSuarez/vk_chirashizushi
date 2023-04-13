#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>
#include "vulkan_renderer.h"
#include "window_manager.h"
#include "darray.h"
#include "vulkan_debugger.h"

static VkInstance instance = NULL;
static const char **extensions;

#define VALIDATION_LAYER_COUNT 1
const char** validation_layers; 


void print_layer_property(VkLayerProperties *prop)
{
    printf("\nLAYER DETAIL:\nlayer name:\t%s\ndescription:\t%s\nimp version:\t%d\nspec version:\t%d\n",
            prop->layerName, prop->description, prop->implementationVersion, prop->specVersion);
}

void init_validation_layer()
{
    char *layer = "VK_LAYER_KHRONOS_validation";
    size_t layer_size = strlen(layer);

    validation_layers = malloc(sizeof(char **) * layer_size);
    validation_layers[0] = layer;
}


int check_validation_layer_support()
{
    unsigned int layer_count;
    VkResult layer_result = vkEnumerateInstanceLayerProperties(&layer_count, NULL);
    if(layer_result != VK_SUCCESS)
    {
        printf("Error getting validation layer count\n");
        return 1;
    }
    VkLayerProperties props[layer_count];
    layer_result = vkEnumerateInstanceLayerProperties(&layer_count, props);
    if(layer_result != VK_SUCCESS)
    {
        printf("Error getting validation layers\n");
        return 1;
    }

    // UNCOMMENT THIS IF YOU WANT TO PRINT PROP LAYER INFO
    for (int i = 0; i < layer_count; i++)
    {
        print_layer_property(&props[i]);
    }

    for(int i = 0; i < VALIDATION_LAYER_COUNT; i++)
    {
        uint8_t found = 0;
        for(int j = 0; j < layer_count; j++)
        {
            if(strcmp(validation_layers[i], props[j].layerName) == 0)
            {
                found = 1;
            }
        }

        if(found == 0)
        {
            printf("ERROR: Cannot find validation layer: %s\n", validation_layers[i]);
            return 1;
        }

        printf("All validation layers are present.\n");
    }


    return 0;
}

int init_vulkan()
{
    init_validation_layer();
    if(check_validation_layer_support() != 0)
    {
        return 1;
    }

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
    if(window_get_extensions(&extension_count, NULL) != 0)
    {
        printf("Error creating extensions count\n");
        return 1;
    }

#ifdef VULKAN_DEBUG
    extension_count ++;
#endif
    extensions = malloc(sizeof(char *) * extension_count);

    if(window_get_extensions(&extension_count, extensions) != 0)
    {
        printf("Error creating extensions\n");
        return 1;
    }
#ifdef VULKAN_DEBUG
    extensions[extension_count -1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
#endif

    VkInstanceCreateInfo instance_info =
    {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_info,
        .enabledExtensionCount = extension_count,
        .ppEnabledExtensionNames = extensions,
        .enabledLayerCount = VALIDATION_LAYER_COUNT,
        .ppEnabledLayerNames = validation_layers
    };
    VkResult instance_result = vkCreateInstance(&instance_info, NULL, &instance);

    if(instance_result != VK_SUCCESS)
    {
        printf("Error creating vulkan instance code: %d", instance_result);
        return 1;
    }

#ifdef VULKAN_DEBUG
    vulkan_debugger_init(instance);
#endif
    return 0;
}

void destroy_instance()
{
#ifdef VULKAN_DEBUG
    vulkan_debugger_destroy(instance);
#endif

    vkDestroyInstance(instance, NULL);
    instance = NULL;
    free(extensions);
    extensions = NULL;
    free(validation_layers);
    validation_layers = NULL;
}
