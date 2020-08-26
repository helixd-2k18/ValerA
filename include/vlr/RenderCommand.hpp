#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

    struct RenderCommandCreateInfo {
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<RayTracing> rayTracing = {};
        vkt::uni_ptr<Rasterization> rasterization = {};
    };

    class RenderCommand : public std::enable_shared_from_this<RenderCommand> { protected: 
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<RayTracing> rayTracing = {};
        vkt::uni_ptr<Rasterization> rasterization = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        RenderCommand() { this->constructor(); };
        RenderCommand(vkt::uni_ptr<Driver> driver, vkt::uni_arg<RenderCommandCreateInfo> info = {}) { this->constructor(driver, info); };
        ~RenderCommand() {};

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<RenderCommandCreateInfo> info = {});

        virtual void setRasterization(vkt::uni_ptr<Rasterization> rasterization = {});
        virtual void setRayTracing(vkt::uni_ptr<RayTracing> rayTracing = {});
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> command);
        virtual void setDescriptorSets(vkt::uni_ptr<PipelineLayout> ilayout = {});
    };

};

namespace vlj {
    class RenderCommand : public Wrap<vlr::RenderCommand> {
        RenderCommand() : Wrap<vlr::RenderCommand>() {};
        RenderCommand(vkt::uni_ptr<vlr::RenderCommand> object) : Wrap<vlr::RenderCommand>(object) {};
        RenderCommand(vkt::uni_ptr<vlr::Driver> driver, vkt::uni_arg<vlr::RenderCommandCreateInfo> info = {}) : Wrap<vlr::RenderCommand>(std::make_shared<vlr::RenderCommand>(driver, info)) {};
        RenderCommand(std::shared_ptr<vlr::RenderCommand> object) : Wrap<vlr::RenderCommand>(object) {};

        //CALLIFY(constructor);
        CALLIFY(setRasterization);
        CALLIFY(setRayTracing);
        CALLIFY(setCommand);
        CALLIFY(setDescriptorSets);
    };
};
