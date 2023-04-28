#include "vulkan_setup.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>
#include "window_manager.h"
#include "vulkan_debugger.h"

static VkInstance instance = NULL;

#define VALIDATION_LAYER_COUNT 1
const char** validation_layers; 

static VkDevice device;
static VkQueue graphics_queue;

static VkSurfaceKHR surface;

int create_swap_chain()
{

}

void choose_presentation_mode(VkPresentModeKHR *present_modes, uint32_t size, VkPresentModeKHR *result)
{
    *result = VK_PRESENT_MODE_FIFO_KHR;

    for(int i = 0; i < size; i++)
    {
        if(present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            *result = VK_PRESENT_MODE_MAILBOX_KHR;
            return;
        }
    }
}

int setup_swap_chain(VkPhysicalDevice device, uint32_t *surface_format_count, uint32_t *surface_present_mode_count, VkPresentModeKHR *out_presentation_mode)
{
    VkSurfaceCapabilitiesKHR surface_capabilities;
    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &surface_capabilities);
    int result_code = 0;
    if(result != VK_SUCCESS)
    {
        printf("[ERROR] Cannot get Surface Capabilites code: %d\n", result);
        result_code = 1;
    }

    result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, surface_format_count, NULL);
    if(result != VK_SUCCESS)
    {
        printf("[ERROR] Cannot get surface formats count code: %d\n", result);
        result_code = 1;
    }
    VkSurfaceFormatKHR *surface_formats = malloc(sizeof(VkSurfaceFormatKHR) * *surface_format_count);
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, surface_format_count, surface_formats);
    if(result != VK_SUCCESS)
    {
        printf("[ERROR] Cannot get surface formats code: %d\n", result);
        result_code = 1;
    }

    result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, surface_present_mode_count, NULL);
    if(result != VK_SUCCESS)
    {
        printf("[ERROR] Cannot get surface present mode count code %d\n", result);
        result_code = 1;

    }
    VkPresentModeKHR *present_modes = malloc(sizeof(VkPresentModeKHR) * *surface_present_mode_count);
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, surface_present_mode_count, present_modes);
    if(result != VK_SUCCESS)
    {
        printf("[ERROR] Cannot get surface present modes code %d\n", result);
        result_code = 1;
    }
    choose_presentation_mode(present_modes, *surface_present_mode_count, out_presentation_mode);
    
    free(surface_formats);
    surface_formats = NULL;

    free(present_modes);
    present_modes = NULL;

    return result_code;
}

void print_layer_property(VkLayerProperties *prop)
{
    printf("\nlayer name:\t%s\ndescription:\t%s\nimp version:\t%d\nspec version:\t%d\n",
            prop->layerName, prop->description, prop->implementationVersion, prop->specVersion);
}
void print_physical_device_properties(VkPhysicalDeviceProperties *device_properties)
{
    printf("api version:\t%d\ndriver version:\t%d\nvendor id:\t%d\ndevice id:\t%d\ndevice type:\t%d\ndevice name:\t%s\n",
        device_properties->apiVersion, device_properties->driverVersion, device_properties->vendorID, device_properties->deviceID, device_properties->deviceType, device_properties->deviceName);
}
void print_physical_family_queue(VkQueueFamilyProperties family_queue_prop)
{
    printf("\nflags\t%d\ncount:\t%d\nvalid bits:\t%d\nimage transfer widht:\t%d\nimage transfer height:\t%d\nimage transfer depth:\t%d\n",
        family_queue_prop.queueFlags, 
        family_queue_prop.queueCount, 
        family_queue_prop.timestampValidBits,
        family_queue_prop.minImageTransferGranularity.width, 
        family_queue_prop.minImageTransferGranularity.height,   
        family_queue_prop.minImageTransferGranularity.depth);
}


void init_surface_creation()
{
    window_manager_info wm_info = get_wm_info();
    VkResult create_surface_result = VK_INCOMPLETE;

#ifdef VK_USE_PLATFORM_XLIB_KHR
        VkXlibSurfaceCreateInfoKHR create_surface_info = 
        {
            .sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
            .pNext = NULL,
            .flags = 0,
            .dpy = wm_info.xlib_display,
            .window = wm_info.xlib_window
        };
        create_surface_result = vkCreateXlibSurfaceKHR(instance, &create_surface_info, NULL, &surface);
        printf("[INFO] Add X11 SURFACE\n");
#endif


#ifdef VK_USE_PLATFORM_WAYLAND_KHR
        VkWaylandSurfaceCreateInfoKHR create_surface_info = 
        {
            .sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
            .pNext = NULL,
            .flags = 0,
            .display = wm_info.wl_display,
            .surface = wm_info.wl_surface
        };
        create_surface_result = vkCreateWaylandSurfaceKHR(instance, &create_surface_info, NULL, &surface);
        printf("[INFO] Add WAYLAND SURFACE\n");
#endif
    printf("[INFO] Create surface result %d\n\n", create_surface_result);
}

int check_physical_device_extension(VkPhysicalDevice physical_device, char **required_extensions, uint32_t required_extensions_count)
{
    uint32_t extensions_count;

    VkResult result = vkEnumerateDeviceExtensionProperties(physical_device, NULL, &extensions_count, NULL);
    if(result != VK_SUCCESS)
    {
        printf("[ERROR] Error enumerating Physical Device extensions code: %d\n", result);
        return 0;
    }
    printf("[INFO] Physical Device extensions Count: %d\n", extensions_count);
    VkExtensionProperties *properties = malloc(sizeof(VkExtensionProperties) * extensions_count);
    result = vkEnumerateDeviceExtensionProperties(physical_device, NULL, &extensions_count, properties);
    if(result != VK_SUCCESS)
    {
        printf("[ERROR] Error getting Physical Device extensions code: %d\n", result);
        return 0;
    }

    /* UNCOMMENT TO PRINT PROPERTIES */
    /*for(int i = 0; i < extensions_count; i++)
    {
        printf("[INFO] Phyiscal Device extension at: %d, Name: %s, Version: %d\n", i, properties[i].extensionName, properties[i].specVersion);
    } */

    uint32_t extensions_found_count = 0;
    for(int i = 0; i < extensions_count; i++)
    {
        for(int j = 0; j < required_extensions_count; j++)
        {
            if(strcmp(properties[i].extensionName, required_extensions[j]) == 0)
            {
                printf("[INFO] Found Extensions at index %d, %s\n", i, properties[i].extensionName);
                extensions_found_count++;
                break;
            }
        }

        if(extensions_found_count >= required_extensions_count)
        {
            printf("[INFO] Found all Physical device Extensions\n");
            break;
        }
    }
    if(extensions_found_count == 0 && required_extensions_count > extensions_found_count)
    {
        printf("[ERROR] Cannot get al the physical device extensions\n");
        return 0;
    }

    return 1;
}

void init_physical_device()
{
    uint32_t devices_count;
    VkResult devices_count_result = vkEnumeratePhysicalDevices(instance, &devices_count, NULL);
    if(devices_count_result != VK_SUCCESS)
    {
        printf("[ERROR] Cannot get physical device count code: %d\n", devices_count_result);
        return;
    }
    if(devices_count == 0)
    {
        printf("[WARNING] There's no physical devices\n");
    }
    VkPhysicalDevice *devices = malloc(sizeof(struct VkPhysicalDevice_T *) * devices_count);
    devices_count_result = vkEnumeratePhysicalDevices(instance, &devices_count, devices);
    if(devices_count_result != VK_SUCCESS)
    {
        printf("[ERROR] Cannot get physical devices code: %d\n", devices_count_result);
        return;
    }

    VkPhysicalDeviceFeatures device_features;
    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDevice selected_device = NULL;
    VkPresentModeKHR presentation_mode;
    
    printf("\ndevices count result:\t%d\ndevices count:\t%d\n", devices_count_result, devices_count);
    for(int i = 0; i < devices_count; i++)
    {
        vkGetPhysicalDeviceFeatures(devices[i], &device_features);
        vkGetPhysicalDeviceProperties(devices[i], &device_properties);

        printf("\nDEVICE PROPERTIES:\n");
        print_physical_device_properties(&device_properties);
        if(device_features.geometryShader == VK_TRUE && device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            printf("\nFound device %s\n", device_properties.deviceName);
            selected_device = devices[i];
            uint32_t surface_format_count;
            uint32_t surface_present_mode_count;
            setup_swap_chain(devices[i], &surface_format_count, &surface_present_mode_count, &presentation_mode);
            printf("[INFO] Surfacae format count: \t%d\nSurface present mode count:\t%d\n", surface_format_count, surface_present_mode_count);
            if(surface_format_count > 0 && surface_present_mode_count > 0)
                break;
            else 
                continue;
        }
    }
    free(devices);
    devices = NULL;
    if(selected_device == NULL)
    {
        printf("[ERROR] Cannot find any suitable graphic device\n");
        return;
    }

    printf("PRESENTATION MODE %d\n", presentation_mode);
    uint32_t family_queue_count;
    vkGetPhysicalDeviceQueueFamilyProperties(selected_device, &family_queue_count, NULL);
    VkQueueFamilyProperties *family_queue = malloc(sizeof(VkQueueFamilyProperties) * family_queue_count + 1);
    vkGetPhysicalDeviceQueueFamilyProperties(selected_device, &family_queue_count, family_queue);
    printf("FAMILY QUEUES\n");

    int found_selected_index = 0;
    int found_present_index = 0;
    int all_extension_supported = 0;

    uint32_t selected_queue_index;
    uint32_t present_queue_index;
    char *physical_device_extensions[] = 
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    uint32_t device_extension_count = (sizeof(physical_device_extensions) / sizeof(char *));
    for(int i = 0; i < family_queue_count; i ++)
    {
        print_physical_family_queue(family_queue[i]);
    }
    for(int i = 0; i < family_queue_count; i ++)
    {
        if(found_selected_index == 0 && family_queue[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            selected_queue_index = i;
            found_selected_index = 1;
        }
        if(found_present_index == 0)
        {
            VkBool32 is_surface_supported = VK_FALSE;
            VkResult is_surface_supported_result = vkGetPhysicalDeviceSurfaceSupportKHR(selected_device, i, surface, &is_surface_supported);
            if(is_surface_supported_result != VK_SUCCESS)
            {
                printf("[ERROR] Window Surface is not supported by physical graphic device result: %d, value %d\n", is_surface_supported_result, is_surface_supported);
                return;
            }
            if(is_surface_supported == VK_TRUE)
            {
                present_queue_index = i;
                found_present_index = 1;
            }
            all_extension_supported = check_physical_device_extension(selected_device, physical_device_extensions, device_extension_count);
        }

        if(found_present_index == 1 && found_selected_index == 1 && all_extension_supported == 1)
        {
            break;
        }
    }

    float queue_priority = 1.0;
    VkDeviceQueueCreateInfo queue_create_info = 
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = NULL,
        .queueFamilyIndex = selected_queue_index,
        .queueCount = 1,
        .pQueuePriorities = &queue_priority
    };

    VkDeviceCreateInfo logical_device_create_info = 
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = NULL,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queue_create_info,
#ifdef VULKAN_DEBUG
        .enabledLayerCount = VALIDATION_LAYER_COUNT,
        .ppEnabledLayerNames = validation_layers,
        .pEnabledFeatures = &device_features,
#endif
        .enabledExtensionCount = device_extension_count,
        .ppEnabledExtensionNames = physical_device_extensions
    };

    if(vkCreateDevice(selected_device, &logical_device_create_info, NULL, &device) != VK_SUCCESS)
    {
        printf("[ERROR] Cannot create a VK_DEVICE\n");
        return;
    }

    vkGetDeviceQueue(device, selected_queue_index, 0, &graphics_queue);
    printf("[INFO] FINISH CREATING DEVICE GRAPHIC QUEUE\n\n");
    free(family_queue);
    family_queue = NULL;
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

    /* UNCOMMENT THIS IF YOU WANT TO PRINT PROP LAYER INFO*/
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

        printf("[INFO] All validation layers are present.\n\n");
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

    unsigned int extension_count = 0;
    if(window_get_extensions(&extension_count, NULL) != 0)
    {
        printf("Error creating extensions count\n");
        return 1;
    }

#ifdef VULKAN_DEBUG
    extension_count ++;
#endif
    printf("[INFO] Extensions count: %d\n", extension_count);

    const char **extensions = malloc(sizeof(char *) * extension_count);

    if(window_get_extensions(&extension_count, extensions) != 0)
    {
        printf("Error creating extensions\n");
        return 1;
    }
#ifdef VULKAN_DEBUG
    extension_count ++;
    extensions[extension_count -1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
#endif

    printf("[INFO] EXTENSIONS LIST\n");
    for(int i = 0; i < extension_count; i++)
    {
        printf("Extension: \t%s\n", extensions[i]);
    }
    printf("[INFO] FINISH EXTENSIONS LIST\n");

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
    init_surface_creation();
    init_physical_device();
    free(extensions);
    extensions = NULL;

    printf("[INFO] FINISH CREATING VULKAN INSTANCE AND DEVICES\n\n");
    return 0;
}

void destroy_instance()
{
#ifdef VULKAN_DEBUG
    vulkan_debugger_destroy(instance);
#endif
    vkDestroySurfaceKHR(instance, surface, NULL);
    vkDestroyDevice(device, NULL);
    device = NULL;
    vkDestroyInstance(instance, NULL);
    instance = NULL;
    free(validation_layers);
    validation_layers = NULL;
}
