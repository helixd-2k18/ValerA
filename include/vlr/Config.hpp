#pragma once


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
#include <memory>
#include <vector>
#include <string>

// 
#include <vkh/core.hpp>
#include <vkh/helpers.hpp>
#include <vkt3/utils.hpp>
#include <vkt3/vector.hpp>
#include <vkt3/image.hpp>
#include <vkt3/fw.hpp>

// 
namespace vlr {
    class Acceleration;
    //class AttributeSet;
    //class BindingSet;
    //class MaterialSet;
    class TextureSet;
    class SamplerSet;
    class BuildCommand;
    class BufferViewSet;
    class RenderCommand;
    class OthersCommand;
    class Interpolation;
    class Rasterization;
    class Geometry;
    class GeometrySet;
    class Framebuffer;
    class Background;
    class Resampling;
    class PipelineLayout;
    //class Driver;
    template<class T = uint8_t> class SetBase_T;
    class SetBase;
    class VertexSet;
    class RayTracing;
    class InstanceSet;
    //class Constants;

    struct DataSetCreateInfo {
        VkDeviceSize count = 1u;
        bool uniform = false;
    };

    struct AccelerationCreateInfo;
    struct PipelineCreateInfo;

    //struct AccelerationCreateInfo {
    //    vkt::uni_ptr<GeometrySet> geometrySet = {};
    //    vkt::uni_ptr<InstanceSet> instanceSet = {};
    //    std::vector<VkDeviceSize> initials = {};
    //};

};
