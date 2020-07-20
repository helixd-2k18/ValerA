#pragma once
#include "./Config.hpp"

namespace vlr {

    class PipelineLayout : public std::enable_shared_from_this<PipelineLayout> { protected: 
        VkPipelineCache cache = {};
        VkPipelineLayout bindings = {}, transform = {}; // Unified Bindings, Transform Feedback 
        VkDescriptorPool pool = {};
        
        // 
        std::vector<VkDescriptorSetLayout> layouts = {};
        std::shared_ptr<Driver> driver = {};
        
    public: 
        PipelineLayout() { this->constructor(); };
        PipelineLayout(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        // 
        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver);

        // 
        VkPipelineCache getPipelineCache() const { return cache; };
        VkPipelineLayout getBindingPipelineLayout() const { return bindings; };
        VkPipelineLayout getTransformPipelineLayout() const { return transform; };
        VkDescriptorPool getDescriptorPool() const { return pool; };

        VkDescriptorSetLayout getDescriptorSetLayout(const uint32_t& I) const { return layouts[I]; };
        VkDescriptorSetLayout getSetLayout() const { return layouts[0u]; };
        VkDescriptorSetLayout getTextureSetLayout() const { return layouts[1u]; };
        VkDescriptorSetLayout getSamplerSetLayout() const { return layouts[2u]; };
        VkDescriptorSetLayout getUniformSetLayout() const { return layouts[4u]; };
        VkDescriptorSetLayout getBufferViewSetLayout() const { return layouts[3u]; };
        VkDescriptorSetLayout getBackgroundSetLayout() const { return layouts[5u]; };
        VkDescriptorSetLayout getFramebufferSetLayout() const { return layouts[6u]; };
    };

};
