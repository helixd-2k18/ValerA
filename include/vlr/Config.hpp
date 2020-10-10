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
#ifndef VKT_CORE_USE_XVK
#define VKT_CORE_USE_XVK
#endif

//
#ifndef VKT_CORE_USE_VMA
#define VKT_CORE_USE_VMA
#endif

//
#ifndef VKT_USE_VULKAN_HPP
#define VKT_USE_VULKAN_HPP
#endif

// 
#ifndef VK_ENABLE_BETA_EXTENSIONS
#define VK_ENABLE_BETA_EXTENSIONS
#endif

//
#include <memory>
#include <vector>
#include <string>

// 
#include <vkt3/core.hpp>
#include <glm/glm.hpp>
#include <vkh/structures.hpp>

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

#pragma pack(push, 1)
    struct MeshIDFlags {
        uint32_t ID : 24, hasTransform : 1, hasNormal : 1, hasTexcoord : 1, hasTangent : 1, translucent : 1, reserved : 3;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    // Stock Material Unit
    struct MaterialUnit {
        glm::vec4 diffuse = glm::vec4(1.f);
        glm::vec4 pbrAGM = glm::vec4(0.f);
        glm::vec4 normal = glm::vec4(0.5f, 0.5f, 1.f, 1.f);
        glm::vec4 emission = glm::vec4(0.f);

        int diffuseTexture = -1;
        int pbrAGMTexture = -1;
        int normalTexture = -1;
        int emissionTexture = -1;

        glm::uvec4 udata = glm::uvec4(0u);
    };
#pragma pack(pop)

#pragma pack(push, 1)
    // Stock Geometry Desc
    struct GeometryDesc {
        glm::mat3x4 transform = glm::mat3x4(1.f);
        uint32_t firstVertex = 0u;
        uint32_t primitiveCount = 0u;
        uint32_t material = 0u;

        // 
        union {
            MeshIDFlags mesh_flags;
            uint32_t mesh_flags_u32 = 0u;
        };

        // We solved to re-port into... 
        uint32_t vertexAttribute = 0u, indexBufferView = ~0u, indexType = VK_INDEX_TYPE_NONE_KHR, reserved = 0u;
        uint32_t attributes[8u] = { 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u };
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct ConstantDesc {
        glm::mat4 projection = glm::mat4(1.f);
        glm::mat4 projectionInv = glm::mat4(1.f);
        glm::mat3x4 modelview = glm::mat3x4(1.f);
        glm::mat3x4 modelviewInv = glm::mat3x4(1.f);
        glm::mat3x4 modelviewPrev = glm::mat3x4(1.f);
        glm::mat3x4 modelviewPrevInv = glm::mat3x4(1.f);
        glm::uvec4 mdata = glm::uvec4(0u);                         // mesh mutation or modification data
        //glm::uvec2 tdata = glm::uvec2(0u), rdata = glm::uvec2(0u); // first for time, second for randoms
        glm::uvec2 tdata = glm::uvec2(0u);
        glm::uvec2 rdata = glm::uvec2(0u);
    };
#pragma pack(pop)

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
        Wrap(vkt::uni_ptr<T> object) : object(object) {};
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
        virtual vkt::uni_ptr<T>& uniPtr() { return object; };
        virtual const vkt::uni_ptr<T>& uniPtr() const { return object; };

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
