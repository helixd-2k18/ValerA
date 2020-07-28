// #
#if defined(_WIN32) && !defined(WIN32)
#define WIN32
#endif

//
#ifdef WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif

//#ifdef OS_WIN
#if (defined(_WIN32) || defined(__MINGW32__) || defined(_MSC_VER_) || defined(__MINGW64__)) 
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#endif

//#ifdef OS_LNX
#ifdef __linux__
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#endif

//
#define ENABLE_EXTENSION_GLM
#define ENABLE_EXTENSION_VMA
#define ENABLE_EXTENSION_RTX
#define GLFW_INCLUDE_VULKAN

//
#ifndef VKT_CORE_ENABLE_XVK
#define VKT_CORE_ENABLE_XVK
#endif

//
#ifndef VKT_CORE_ENABLE_VMA
#define VKT_CORE_ENABLE_VMA
#endif

//
#ifndef ENABLE_VULKAN_HPP
#define ENABLE_VULKAN_HPP
#endif

// 
#ifndef VK_ENABLE_BETA_EXTENSIONS
#define VK_ENABLE_BETA_EXTENSIONS
#endif

// 
#define VKT_CORE_ENABLE_VMA
#define VKT_CORE_ENABLE_XVK
#define VKT_CORE_ENABLE_VMA

//
#define VMA_IMPLEMENTATION
//#define TINYEXR_IMPLEMENTATION

//#include "./vlr/Config.hpp"

#include "vma/vk_mem_alloc.h"