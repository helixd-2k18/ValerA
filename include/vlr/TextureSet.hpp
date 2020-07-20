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
        virtual void constructor(vkt::uni_ptr<Driver> driver) {};
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier = false);
        virtual void pushImage(const vkt::uni_arg<vkt::ImageRegion>& image) {
            this->images.push_back(image);
        };
        virtual void resetImages(){
            this->images.resize(0ull);
        };

        // 
        vkt::ImageRegion& operator[](const uint32_t& I) { return images[I]; };
        const vkt::ImageRegion& operator[](const uint32_t& I) const { return images[I]; };
    };

};
