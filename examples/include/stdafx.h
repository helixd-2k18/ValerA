#pragma once

// 
#ifndef STDAFX_H
#define STDAFX_H

// 
#define VKT_ENABLE_GLFW_SUPPORT
#define GLM_FORCE_SWIZZLE 
#define GLM_SWIZZLE_XYZW 
#define GLM_SWIZZLE_STQP

// 
#define VK_NO_PROTOTYPES
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#define VKT_ENABLE_GLFW_SURFACE
#define VKT_ENABLE_GLFW_LINKED

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
#include <vlr/Constants.hpp>
#include <vlr/BufferViewSet.hpp>
#include <vlr/MaterialSet.hpp>
#include <vlr/TextureSet.hpp>
#include <vlr/SamplerSet.hpp>
#include <vlr/Background.hpp>

// 
#include <vlr/GeometrySet.hpp>
#include <vlr/Acceleration.hpp>
#include <vlr/BuildCommand.hpp>

// 
#include <vlr/RayTracing.hpp>
#include <vlr/Rasterization.hpp>
#include <vlr/RenderCommand.hpp>

// 
#include <misc/tiny_gltf.h>
#include <misc/tinyexr.h>
#include <vkh/helpers.hpp>

#endif
