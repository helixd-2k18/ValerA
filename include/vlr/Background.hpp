#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

    // Not Only Background Images, but Native Samplers
    class Background : public std::enable_shared_from_this<Background> {
    protected: friend PipelineLayout;
        vkt::ImageRegion background = {};
        vkt::uni_ptr<Driver> driver = {};
        VkDescriptorSet set = {}; bool updated = false;
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};

    public: 
        Background() { this->constructor(); };
        Background(vkt::uni_ptr<Driver> driver, vkt::uni_arg<vkt::ImageRegion> image = vkt::ImageRegion{}) { this->constructor(driver, image); };
        ~Background() {};

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<vkt::ImageRegion> image = vkt::ImageRegion{});

        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        //virtual void setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier = false);
        virtual void setImage(vkt::uni_arg<vkt::ImageRegion> image = {}) { this->background = image.ref(); };
    };

};

namespace vlj {
    class Background : public Wrap<vlr::Background> {
    public:
        Background() : Wrap<vlr::Background>() {};
        Background(vkt::uni_ptr<vlr::Background> object) : Wrap<vlr::Background>(object) {};
        Background(vkt::uni_ptr<vlr::Driver> driver, vkt::uni_arg<vkt::ImageRegion> image = vkt::ImageRegion{}) : Wrap<vlr::Background>(std::make_shared<vlr::Background>(driver, image)) {};
        Background(std::shared_ptr<vlr::Background> object) : Wrap<vlr::Background>(object) {};

        //CALLIFY(constructor);
        CALLIFY(createDescriptorSet);
        CALLIFY(setImage);
    };
};
