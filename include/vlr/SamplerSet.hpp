#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

    class SamplerSet : public std::enable_shared_from_this<SamplerSet> { protected: friend RayTracing; friend PipelineLayout;
        VkDescriptorSet set = {}; bool updated = false;
        std::vector<VkSampler> samplers = {};
        vkt::uni_ptr<Driver> driver = {};
        
    public: 
        SamplerSet() { this->constructor(); };
        SamplerSet(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };
        ~SamplerSet() {
            const auto device = driver->getDeviceDispatch();
            if (set) { vkh::handleVk(device->vkFreeDescriptorSets(device->handle, driver->getDescriptorPool(), 1u, &set)); set = {}; };
        };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            this->driver = driver;
        };
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        //virtual void setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier = false);
        virtual intptr_t pushSampler(const VkSampler& sampler) { intptr_t ptr = this->samplers.size(); this->samplers.push_back(sampler); return ptr; };
        virtual void resetSamplers(){ this->samplers.clear(); this->samplers.resize(0ull); };

        //
        VkSampler& get(const uint32_t& I = 0u) { if (samplers.size() <= I) { samplers.resize(I + 1); }; return samplers[I]; };
        const VkSampler& get(const uint32_t& I = 0u) const { return samplers[I]; };

        // 
        VkSampler& operator[](const uint32_t& I) { return get(I); };
        const VkSampler& operator[](const uint32_t& I) const { return get(I); };
    };

};

namespace vlj {
    class SamplerSet : public Wrap<vlr::SamplerSet> {
    public:
        SamplerSet() : Wrap<vlr::SamplerSet>() {};
        SamplerSet(vkt::uni_ptr<vlr::SamplerSet> object) : Wrap<vlr::SamplerSet>(object) {};
        SamplerSet(vkt::uni_ptr<vlr::Driver> driver) : Wrap<vlr::SamplerSet>(std::make_shared<vlr::SamplerSet>(driver)) {};
        SamplerSet(std::shared_ptr<vlr::SamplerSet> object) : Wrap<vlr::SamplerSet>(object) {};

        //CALLIFY(constructor);
        CALLIFY(createDescriptorSet);
        CALLIFY(pushSampler);
        CALLIFY(resetSamplers);
        CALLIFY(get);
    };
};
