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
        virtual void resetSampler(){ this->samplers.clear(); this->samplers.resize(0ull); };

        //
        VkSampler& get(const uint32_t& I = 0u) { return samplers[I]; };
        const VkSampler& get(const uint32_t& I = 0u) const { return samplers[I]; };

        // 
        VkSampler& operator[](const uint32_t& I) { return samplers[I]; };
        const VkSampler& operator[](const uint32_t& I) const { return samplers[I]; };
    };

};

namespace vlj {
    class SamplerSet : public Wrap<vlr::SamplerSet> {
        SamplerSet() : Wrap<vlr::SamplerSet>() {};
        SamplerSet(vkt::uni_ptr<vlr::SamplerSet> object) : Wrap<vlr::SamplerSet>(object) {};
        SamplerSet(vkt::uni_ptr<vlr::Driver> driver) : Wrap<vlr::SamplerSet>(std::make_shared<vlr::SamplerSet>(driver)) {};
        SamplerSet(std::shared_ptr<vlr::SamplerSet> object) : Wrap<vlr::SamplerSet>(object) {};

        //CALLIFY(constructor);
        CALLIFY(createDescriptorSet);
        CALLIFY(pushSampler);
        CALLIFY(resetSampler);
        CALLIFY(get);
    };
};
