#pragma once

// 
#define VK_NO_PROTOTYPES

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
#include <vkt3/fw.hpp>
#include <vkt3/utils.hpp>
#include <vkt3/vector.hpp>
#include <vkt3/image.hpp>
#include <vkh/helpers.hpp>

// 
#define CALLIFY(NAME)\
     template<class... A> inline decltype(auto) NAME(A... args) { return object->NAME(args...); };\
     template<class... A> inline decltype(auto) NAME(A... args) const { return object->NAME(args...); }

// 
#define uPTR(NAME) vkt::uni_ptr<NAME>
#ifdef NATIVE_SHARED_PTR
//#define uPTR(NAME) std::shared_ptr<NAME>
#define uTHIS shared_from_this()
#else // For Java Applications compatibility
//#define uPTR(NAME) NAME*
#define uTHIS this
#endif

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
        bool enableCPU = true;
        bool enableGL = false;
    };

    struct AccelerationCreateInfo;
    struct PipelineCreateInfo;

    //struct AccelerationCreateInfo {
    //    vkt::uni_ptr<GeometrySet> geometrySet = {};
    //    vkt::uni_ptr<InstanceSet> instanceSet = {};
    //    std::vector<VkDeviceSize> initials = {};
    //};
};

// For JavaCpp!
namespace vlj {
    template<class T>
    class Wrap { public:
        Wrap() {};
        Wrap(const Wrap<T>& wrap) : object(wrap.object) {};
        Wrap(std::shared_ptr<T> object) : object(object) {};
        Wrap(T* object) : object(object) {};
        Wrap(T& object) : object(&object) {};

        // 
        virtual Wrap<T>& operator=(vkt::uni_ptr<T> object) { this->object = object; return *this; };
        virtual Wrap<T>& operator=(std::shared_ptr<T> object) { this->object = object; return *this; };
        virtual Wrap<T>& operator=(Wrap<T> object) { this->object = object.object; return *this; };

        // 
        virtual operator T& () { return object.ref(); };
        //virtual operator const T& () const { return object.ref(); };

        // 
        virtual operator T* () { return object.ptr(); };
        virtual operator const T* () const { return object.ptr(); };

        // now required explicit...
        virtual explicit operator std::shared_ptr<T>& () { return object.get_shared(); };
        virtual explicit operator const std::shared_ptr<T>& () const { return object.get_shared(); };

        // 
        virtual operator vkt::uni_ptr<T>& () { return object; };
        virtual operator const vkt::uni_ptr<T>& () const { return object; };

        // 
        virtual T* operator->() { return object.ptr(); };
        virtual T& operator*() { return object.ref(); };
        virtual const T* operator->() const { return object.ptr(); };
        //virtual const T& operator*() const { return object.ref(); };

        // Due Explicit
        virtual std::shared_ptr<T>& sharedPtr() { return object.get_shared(); };
        virtual const std::shared_ptr<T>& sharedPtr() const { return object.get_shared(); };

    // 
    protected: friend Wrap; friend T; // 
        vkt::uni_ptr<T> object = {};
    };

    class Acceleration;
    class TextureSet;
    class SamplerSet;
    class BuildCommand;
    class BufferViewSet;
    class RenderCommand;
    class OthersCommand;
    class Rasterization;
    class Geometry;
    class GeometrySet;
    class Framebuffer;
    class Background;
    class Resampling;
    class PipelineLayout;
    template<class T = uint8_t> class SetBase_T;
    class SetBase;
    class VertexSet;
    class RayTracing;
    class InstanceSet;

    struct AccelerationCreateInfo;
    struct PipelineCreateInfo;
};
