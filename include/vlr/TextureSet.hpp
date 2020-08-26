#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {
    
    class TextureSet : public std::enable_shared_from_this<TextureSet> { protected: friend RayTracing; friend PipelineLayout;
        VkDescriptorSet set = {}; bool updated = false;
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};
        std::vector<vkt::ImageRegion> images = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        TextureSet() { this->constructor(); };
        TextureSet(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };
        ~TextureSet() {};

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            this->driver = driver;
        };
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        //virtual void setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier = false);
        virtual void pushImage(vkt::uni_arg<vkt::ImageRegion> image) { this->images.push_back(image); };
        virtual void resetImages(){ this->images.resize(0ull); };

        //
        vkt::ImageRegion& get(const uint32_t& I = 0u) { return images[I]; };
        const vkt::ImageRegion& get(const uint32_t& I = 0u) const { return images[I]; };

        // 
        vkt::ImageRegion& operator[](const uint32_t& I) { return images[I]; };
        const vkt::ImageRegion& operator[](const uint32_t& I) const { return images[I]; };
    };

};

namespace vlj {
    class TextureSet : public Wrap<vlr::TextureSet> {
        TextureSet() : Wrap<vlr::TextureSet>() {};
        TextureSet(vkt::uni_ptr<vlr::TextureSet> object) : Wrap<vlr::TextureSet>(object) {};
        TextureSet(vkt::uni_ptr<vlr::Driver> driver) : Wrap<vlr::TextureSet>(std::make_shared<vlr::TextureSet>(driver)) {};

        //CALLIFY(constructor);
        CALLIFY(get);
    };
};
