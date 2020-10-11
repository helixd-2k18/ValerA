#pragma once

// 
#ifndef STDAFX_H
#define STDAFX_H

// 
#define VKT_USE_GLFW
#define VKT_GLFW_SURFACE
#define VKT_GLFW_LINKED

// 
#define GLM_FORCE_SWIZZLE 
#define GLM_SWIZZLE_XYZW 
#define GLM_SWIZZLE_STQP

// 
#define VK_NO_PROTOTYPES
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0

// 
//#define VK_NO_PROTOTYPES
#include <vlr/Config.hpp>
#include <vlr/Driver.hpp>

// #
#if defined(_WIN32) && !defined(WIN32)
#define WIN32
#endif

// 
#ifdef WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif

//
//#define VMA_IMPLEMENTATION
//#define TINYGLTF_IMPLEMENTATION
//#define TINYEXR_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION

//
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// 
#include <vlr/Renderer.hpp>

// 
#include <misc/tiny_gltf.h>
#include <misc/tinyexr.h>
#include <vkh/helpers.hpp>

// still required for main.cpp 
#include <vkt3/essential.hpp>

#endif
