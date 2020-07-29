#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./PipelineLayout.hpp"
#include "./Framebuffer.hpp"
#include "./Geometry.hpp"

namespace vlr {

    class Resampling : public std::enable_shared_from_this<Resampling> { protected: friend RayTracing;
        vkt::uni_ptr<VertexSet> vertexSet = {};
        vkt::uni_ptr<Geometry> geometry = {};
        vkt::uni_ptr<Interpolation> interpolation = {};
        vkt::uni_ptr<PipelineLayout> layout = {};
        //vkt::uni_arg<PipelineCreateInfo> info = {};
        vkt::uni_ptr<TextureSet> textureSet = {};
        vkt::uni_ptr<SamplerSet> samplerSet = {};
        vkt::uni_ptr<MaterialSet> materialSet = {};
        vkt::uni_ptr<Framebuffer> framebuffer = {};
        vkt::uni_ptr<Driver> driver = {};
        VkPipeline pipeline = VK_NULL_HANDLE;

        // 
        std::vector<vkh::VkPipelineShaderStageCreateInfo> stages = {};
        vkh::VsGraphicsPipelineCreateInfoConstruction pipelineInfo = {};
        VkPipelineRasterizationConservativeStateCreateInfoEXT conserv = {};

    public: 
        Resampling() { this->constructor(); };
        Resampling(vkt::uni_ptr<Driver> driver, vkt::uni_arg<PipelineCreateInfo> info) { this->constructor(driver, info); };
        ~Resampling() {};

        // 
        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<PipelineCreateInfo> info);
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> rasterCommand, const glm::uvec4& meta = glm::uvec4(0u));
        
    };

};
