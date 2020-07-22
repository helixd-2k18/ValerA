#pragma once
#include "./Config.hpp"
#include "./Framebuffer.hpp"
#include "./SetBase.hpp"
#include "./BufferViewSet.hpp"

namespace vlr {

    // i.e. extended float3x4
    struct RayData {
        glm::vec3 origin = glm::vec3(0.f); uint32_t reserved = 0u;
        glm::vec3 direct = glm::vec3(0.f); glm::u8vec4 meta = glm::u8vec4(0u); // 0u - flags, 1u - lifetime
        glm::uvec2 color = glm::uvec2(0u); glm::uvec2 emission = glm::uvec2(0u); // Packed FP16
    };

    // i.e. float2x4
    struct HitData {
        glm::uvec3 indices = glm::uvec3(0u); uint32_t rayID = 0u; // 
        glm::vec2 barycentric = glm::vec2(0.f); float extra = 0.f;
        glm::u8vec4 meta = glm::u8vec4(0u); // 0u - flags, reserved
    };

    // i.e. vec4 
    struct ColorData {
        glm::vec3 emission = glm::vec3(0.f); // Packed FP16
        uint32_t next = ~0u;
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
        // TODO: Accumulation Shader (pick up all hits)

        // 
        vkt::uni_ptr<SetBase_T<uint32_t>> counters = {};
        vkt::uni_ptr<SetBase_T<ColorData>> colorChainData = {};
        vkt::uni_ptr<SetBase_T<HitData>> hitData = {};
        vkt::uni_ptr<SetBase_T<RayData>> rayDataFlip0 = {};
        vkt::uni_ptr<SetBase_T<RayData>> rayDataFlip1 = {};
        vkt::uni_ptr<BufferViewSet> rayDataSetFlip0 = {};
        vkt::uni_ptr<BufferViewSet> rayDataSetFlip1 = {};

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
