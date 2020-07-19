#pragma once
#include "./Config.hpp"

namespace vlr {

    struct RenderPass {
        VkFramebuffer framebuffer = {};
        VkRenderPass renderPass = {};
        std::vector<vkh::VkPipelineColorBlendAttachmentState> blendStates = {};
        std::vector<vkh::VkClearValue> clearValues = {};
    };

    class Framebuffer : public std::enable_shared_from_this<Framebuffer> { protected: 
    RenderPass rasterFBO = {}, resampleFBO = {};
        vkt::ImageRegion depthStencilImage = {};
        std::vector<vkt::ImageRegion> currentsImages = {};  // Current Frame
        std::vector<vkt::ImageRegion> previousImages = {};  // Previous Frame
        std::vector<vkt::ImageRegion> resampledImages = {}; // Resampled Previous Frame
        std::vector<vkt::ImageRegion> rasterImages = {};    // Rasterized Frame
        VkRect2D scissor = {}; VkViewport viewport = {};    // 
        uint32_t width = 1920u, height = 1200u;

    public: 
        Framebuffer() { this->constructor(); };
        Framebuffer(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        void constructor() {};
        void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
