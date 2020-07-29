#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

    // Not Only Background Images, but Native Samplers
    class Background : public std::enable_shared_from_this<Background> { protected: 
        vkt::ImageRegion background = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        Background() { this->constructor(); };
        Background(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };
        ~Background() {};

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };

        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier = false);
        virtual void setImage(vkt::ImageRegion image) { this->background = image; };
    };

};
