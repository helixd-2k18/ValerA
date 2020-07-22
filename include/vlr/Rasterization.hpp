#pragma once
#include "./Config.hpp"
#include "./PipelineLayout.hpp"
#include "./Framebuffer.hpp"
#include "./Geometry.hpp"

namespace vlr {

    struct PipelineCreateInfo {
        vkt::uni_ptr<PipelineLayout> pipelineLayout = {};
        vkt::uni_ptr<Framebuffer> framebuffer = {};
    };

    class Rasterization : public std::enable_shared_from_this<Rasterization> { protected: friend RayTracing;
        vkt::uni_ptr<VertexSet> vertexSet = {};
        vkt::uni_ptr<Geometry> geometry = {};
        vkt::uni_ptr<Interpolation> interpolation = {};
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<Driver> driver = {};
        vkt::uni_arg<PipelineCreateInfo> info = {};
        VkPipeline pipeline = VK_NULL_HANDLE;

        // 
        std::vector<vkh::VkPipelineShaderStageCreateInfo> stages = {};
        vkh::VsGraphicsPipelineCreateInfoConstruction pipelineInfo = {};
        VkPipelineRasterizationConservativeStateCreateInfoEXT conserv = {};

    public: 
        Rasterization() { this->constructor(); };
        Rasterization(vkt::uni_ptr<Driver> driver, vkt::uni_arg<PipelineCreateInfo> info) { this->constructor(driver, info); };

        // 
        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<PipelineCreateInfo> info);
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> rasterCommand, glm::uvec4 meta = glm::uvec4(0u));
        
    };

};
