#include "../headers/vulkan_debugger.h"
#include <stdio.h>

#define CREATE_INFO_SEVERITY VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT \
                            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT \
                            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT

#define CREATE_INFO_TYPE VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT \
                        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT \
                        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT

#define CREATE_DEBUGGER_PFN_NAME "vkCreateDebugUtilsMessengerEXT"
#define DESTROY_DEBUGGER_PFN_NAME "vkDestroyDebugUtilsMessengerEXT"

static VkDebugUtilsMessengerEXT debug_messenger;


VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback (VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, 
    VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_data)
{
    if(message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        printf("\nvalidation layer: %s\n", callback_data->pMessage);
        // continue with vulkan layer 
        return VK_FALSE;
    }
    // abort vulkan layer with exception
    return VK_TRUE;
}

void vulkan_debugger_init(VkInstance instance)
{
    VkDebugUtilsMessengerCreateInfoEXT create_info =
    {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = CREATE_INFO_SEVERITY,
        .messageType = CREATE_INFO_TYPE,
        .pfnUserCallback = debug_callback,
        .pUserData = NULL
    };
    PFN_vkCreateDebugUtilsMessengerEXT func_ptr = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, CREATE_DEBUGGER_PFN_NAME);
    if(func_ptr == NULL)
    {
        printf("\nERROR: Cannot find debugger function %s\n", CREATE_DEBUGGER_PFN_NAME);
        return;
    }
    VkResult creation_result = func_ptr(instance, &create_info, NULL, &debug_messenger);
    if(creation_result != VK_SUCCESS)
    {
        printf("ERROR: Error creating Vulkan Debugger code: %d\n", creation_result);
        return;
    }

    printf("\nINFO: Vulkan Debugger created\n");
}

void vulkan_debugger_destroy(VkInstance instance)
{
    PFN_vkDestroyDebugUtilsMessengerEXT func_ptr = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, DESTROY_DEBUGGER_PFN_NAME);
    if(func_ptr == NULL)
    {
        printf("ERROR: Cannot get function pointer to %s\n", DESTROY_DEBUGGER_PFN_NAME);
        return;
    }
    func_ptr(instance, debug_messenger, NULL);
}