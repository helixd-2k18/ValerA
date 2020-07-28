#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./PipelineLayout.hpp"

namespace vlr {

    struct PipelineCreateInfo {
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<Framebuffer> framebuffer = {};
        vkt::uni_ptr<InstanceSet> instanceSet = {};
        vkt::uni_ptr<Constants> constants = {};
        std::vector<vkt::uni_ptr<GeometrySet>> geometrySets = {}; // For Rasterization
        uint32_t geometryID = 0u; // For Rasterization, Use That Geometry
        uint32_t instanceID = 0u; // For Rasterization, Pick Up Transform
    };

    struct RayTracingCreateInfo {
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<Framebuffer> framebuffer = {};
        vkt::uni_ptr<Acceleration> accelerationTop = {}; // Top Level for Ray Tracing
        vkt::uni_ptr<Constants> constants = {};
        std::vector<vkt::uni_ptr<Acceleration>> accelerations = {}; // Bottom Levels
    };

    struct RenderPass {
        VkFramebuffer framebuffer = {};
        VkRenderPass renderPass = {};
        
        std::vector<vkh::VkPipelineColorBlendAttachmentState> blendStates = {};
        std::vector<vkh::VkClearValue> clearValues = {};
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};
    };

    class Framebuffer : public std::enable_shared_from_this<Framebuffer> { protected: friend Rasterization; friend Resampling; friend RayTracing; friend PipelineLayout;
        RenderPass rasterFBO = {}, resampleFBO = {}; VkDescriptorSet set = {};
        vkt::ImageRegion depthStencilImage = {}, atomicMapping = {};
        std::vector<VkSampler> samplers = {};
        std::vector<vkt::ImageRegion> currentsImages = {};  // Current Frame
        std::vector<vkt::ImageRegion> previousImages = {};  // Previous Frame
        std::vector<vkt::ImageRegion> resampleImages = {}; // Resampled Previous Frame
        std::vector<vkt::ImageRegion> rasterImages = {};    // Rasterized Frame
        VkRect2D scissor = {}; VkViewport viewport = {};    // 
        uint32_t width = 1920u, height = 1200u;
        vkt::uni_ptr<Driver> driver = {};
        bool updated = false;

    public: 
        Framebuffer() { this->constructor(); };
        Framebuffer(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        // 
        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver);
        virtual void createRenderPass();
        virtual void createFramebuffer(uint32_t width = 1920u, uint32_t height = 1200u);
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> layout);
    };

};
