#pragma once
#include "./Config.hpp"

namespace vlr {

    // Not Only Background Images, but Native Samplers
    class Background : public std::enable_shared_from_this<Background> { protected: 
        vkt::ImageRegion background = {};
        std::vector<VkSampler> samplers = {};
        std::shared_ptr<Driver> driver = {};

    public: 
        Background() { this->constructor(); };
        Background(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };

        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier = false);
    };

};
