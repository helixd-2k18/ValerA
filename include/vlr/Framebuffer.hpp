#pragma once
#include "./Config.hpp"

namespace vlr {

    class Framebuffer : public std::enable_shared_from_this<Framebuffer> { protected: 
        std::vector<vkt::ImageRegion> flip0images = {};
        std::vector<vkt::ImageRegion> flip1images = {};
        std::vector<vkt::ImageRegion> rasterimages = {};
        vkt::ImageRegion depthStencilImage = {};
        uint32_t width = 1920, height = 1200;
        VkRect2D scissor = {};
        VkViewport viewport = {};
        std::vector<vkh::VkPipelineColorBlendAttachmentState> blendStates = {};
        std::vector<vkh::VkClearValue> clearValues = {}; 

    public: 
        Framebuffer() { this->constructor(); };

        void constructor() {
            
        };
    };

};
