#pragma once

// 
#define VKT_CORE_ENABLE_VMA
#define VKT_CORE_ENABLE_XVK
#define VK_ENABLE_BETA_EXTENSIONS

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
    class AttributeSet;
    class BindingSet;
    class MaterialSet;
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
    class Driver;
    template<class T = uint8_t> class SetBase_T;
    class SetBase;
    class VertexSet;
    class RayTracing;


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
