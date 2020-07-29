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
