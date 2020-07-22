#pragma once
#include "./Config.hpp"
#include "./Framebuffer.hpp"
#include "./SetBase.hpp"
#include "./BufferViewSet.hpp"

namespace vlr {

    struct RayData {

    };

    struct HitData {
        
    };

    class RayTracing : public std::enable_shared_from_this<RayTracing> { protected: 
        vkt::uni_ptr<Acceleration> accelerationTop = {}; // Top Level Acceleration Structure
        vkt::uni_ptr<VertexSet> vertexSet = {}; // Vertex Set 
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<TextureSet> textureSet = {};
        vkt::uni_ptr<SamplerSet> samplerSet = {};
        vkt::uni_ptr<MaterialSet> materialSet = {};
        vkt::uni_ptr<Framebuffer> framebuffer = {};
        vkt::uni_ptr<Driver> driver = {};

        //std::vector<vkt::uni_ptr<GeometrySet>> geometries = {}; // Not Necessary, inbound with `vkt::uni_ptr<Acceleration>`
        std::vector<vkh::VkPipelineShaderStageCreateInfo> stages = {};
        std::vector<vkt::uni_ptr<Acceleration>> accelerations = {};
        VkPipeline generation = VK_NULL_HANDLE, interpolation = VK_NULL_HANDLE, intersection = VK_NULL_HANDLE, finalize = VK_NULL_HANDLE;
        
        //
        vkt::uni_ptr<SetBase_T<HitData>> hitData = {};
        vkt::uni_ptr<SetBase_T<RayData>> rayDataFlip0 = {};
        vkt::uni_ptr<SetBase_T<RayData>> rayDataFlip1 = {};
        vkt::uni_ptr<BufferViewSet> rayDataSetFlip0 = {};
        vkt::uni_ptr<BufferViewSet> rayDataSetFlip1 = {};
        vkt::uni_ptr<SetBase_T<uint32_t>> counters = {};

    public: 
        RayTracing() { this->constructor(); };
        RayTracing(vkt::uni_ptr<Driver> driver, vkt::uni_arg<PipelineCreateInfo> info = {}) { this->constructor(driver, info); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<PipelineCreateInfo> info = {});
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> rasterCommand, const glm::uvec4& meta = glm::uvec4(0u));
        virtual void swapRayData() {
            std::swap(this->rayDataSetFlip0, this->rayDataSetFlip1); // Swap
            this->layout->bound[12u] = this->rayDataSetFlip0->set;
        };
    };

};
