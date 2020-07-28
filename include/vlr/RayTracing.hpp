#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./Framebuffer.hpp"
#include "./SetBase.hpp"
#include "./BufferViewSet.hpp"
#include "./Constants.hpp"
#include "./Acceleration.hpp"

namespace vlr {

#pragma pack(push, 1)
    // i.e. extended float3x4
    struct RayData {
        glm::vec3 origin = glm::vec3(0.f); uint32_t reserved = 0u;
        glm::vec3 direct = glm::vec3(0.f); glm::u8vec4 meta = glm::u8vec4(0u); // 0u - flags, 1u - lifetime
        glm::uvec2 color = glm::uvec2(0u); glm::uvec2 emission = glm::uvec2(0u); // Packed FP16
    };

    // i.e. float2x4
    struct HitData {
        glm::uvec3 indices = glm::uvec3(0u); uint32_t rayID = 0u; // 
        glm::vec2 barycentric = glm::vec2(0.f); uint32_t pixelID = 0u;
        vkh::uint24_t meshID = 0u; uint8_t meta = uint8_t(0u);
    };

    // i.e. vec4 
    struct ColorData { // No needs when used `GL_EXT_shader_atomic_float` 
        glm::vec3 emission = glm::vec3(0.f); // RGB Color by RGB32F, Sample Always 1S
        uint32_t next = ~0u; // Also, can be replaced by Pixel ID when used Accumulation Shader
    };
#pragma pack(pop)

    class RayTracing : public std::enable_shared_from_this<RayTracing> { protected: friend RayTracing;
        vkt::uni_ptr<Acceleration> accelerationTop = {}; // Top Level Acceleration Structure
        vkt::uni_ptr<VertexSet> vertexSet = {}; // Vertex Set 
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<TextureSet> textureSet = {};
        vkt::uni_ptr<SamplerSet> samplerSet = {};
        vkt::uni_ptr<MaterialSet> materialSet = {};
        vkt::uni_ptr<Framebuffer> framebuffer = {};
        vkt::uni_ptr<Constants> constants = {};
        vkt::uni_ptr<Driver> driver = {};
        
        // 
        std::vector<vkh::VkPipelineShaderStageCreateInfo> stages = {};
        std::vector<vkt::uni_ptr<Acceleration>> accelerations = {};
        VkPipeline generation = VK_NULL_HANDLE, interpolation = VK_NULL_HANDLE, intersection = VK_NULL_HANDLE, finalize = VK_NULL_HANDLE, resample = VK_NULL_HANDLE;
        // TODO: Accumulation Shader (pick up all hits)

        // 
        vkt::uni_ptr<BufferViewSet> geometriesDescs = {}; // Buffers with GeometryDesc
        vkt::uni_ptr<BufferViewSet> interpolations = {};

        // 
        vkt::uni_ptr<SetBase_T<uint32_t>> counters = {};
        vkt::uni_ptr<SetBase_T<ColorData>> colorChainData = {};
        vkt::uni_ptr<SetBase_T<HitData>> hitData = {};
        vkt::uni_ptr<SetBase_T<RayData>> rayDataFlip0 = {};
        vkt::uni_ptr<SetBase_T<RayData>> rayDataFlip1 = {};
        vkt::uni_ptr<BufferViewSet> rayDataSetFlip0 = {};
        vkt::uni_ptr<BufferViewSet> rayDataSetFlip1 = {};

        // For TOP Level
        //vkt::uni_ptr<InstanceSet> instanceSet = {}; // Used By Top Level
        //std::vector<vkt::uni_ptr<GeometrySet>> geometries = {}; // Not Necessary, inbound with `vkt::uni_ptr<Acceleration>`, Used By Bottom Levels

    public: 
        RayTracing() { this->constructor(); };
        RayTracing(vkt::uni_ptr<Driver> driver, vkt::uni_arg<RayTracingCreateInfo> info = {}) { this->constructor(driver, info); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<RayTracingCreateInfo> info = {});
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> rasterCommand, const glm::uvec4& meta = glm::uvec4(0u));
        virtual void setDescriptorSets(vkt::uni_ptr<PipelineLayout> ilayout = {});
        virtual void swapRayData() {
            std::swap(this->rayDataSetFlip0, this->rayDataSetFlip1); // Swap
            this->layout->bound[12u] = this->rayDataSetFlip0->set;
        };

        /* Deferred Operations */ 

        // Set Top Level Acceleration
        virtual void setAccelerationTop(vkt::uni_ptr<Acceleration> accelerationTop) {
            this->accelerationTop = accelerationTop;
        };

        // Push Bottom Level Acceleration (with Geometry Set)
        virtual void pushAcceleration(vkt::uni_ptr<Acceleration> acceleration) {
            this->accelerations.push_back(acceleration);
        };

        // Set Constants Buffer
        virtual void setConstants(vkt::uni_ptr<Constants> constants) {
            this->constants = constants;
        };
    };

};
