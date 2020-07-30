#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

    class SamplerSet : public std::enable_shared_from_this<SamplerSet> { protected: friend RayTracing; friend PipelineLayout;
        VkDescriptorSet set = {}; bool updated = false;
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};
        std::vector<VkSampler> samplers = {};
        vkt::uni_ptr<Driver> driver = {};
        
    public: 
        SamplerSet() { this->constructor(); };
        SamplerSet(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };
        ~SamplerSet() {};

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            this->driver = driver;
        };
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        //virtual void setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier = false);
        virtual void pushSampler(const VkSampler& sampler) { this->samplers.push_back(sampler); };
        virtual void resetSampler(){ this->samplers.resize(0ull); };

        // 
        VkSampler& operator[](const uint32_t& I) { return samplers[I]; };
        const VkSampler& operator[](const uint32_t& I) const { return samplers[I]; };
    };

};
