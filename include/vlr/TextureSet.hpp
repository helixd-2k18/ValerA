#pragma once
#include "./Config.hpp"

namespace vlr {
    
    class TextureSet : public std::enable_shared_from_this<TextureSet> { protected: 
        VkDescriptorSet set = {}; bool updated = false;
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};
        std::shared_ptr<Driver> driver = {};
        std::vector<vkt::ImageRegion> images = {};

    public: 
        TextureSet() { this->constructor(); };
        TextureSet(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };

        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier = false);
    };

};
