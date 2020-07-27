#include "./vlr/RenderCommand.hpp"
#include "./vlr/Rasterization.hpp"
#include "./vlr/RayTracing.hpp"

namespace vlr {
    
    void RenderCommand::constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<RenderCommandCreateInfo> info) {
        if (info.has()) {
            this->rasterization = info->rasterization;
            this->rayTracing = info->rayTracing;
            this->layout = info->layout;
        };
    };

    void RenderCommand::setRasterization(vkt::uni_ptr<Rasterization> rasterization) {
        this->rasterization = rasterization;
    };
    
    void RenderCommand::setRayTracing(vkt::uni_ptr<RayTracing> rayTracing) {
        this->rayTracing = rayTracing;
    };

    void RenderCommand::setCommand(vkt::uni_arg<VkCommandBuffer> command) {
        this->setDescriptorSets();
        if (this->rasterization.has()) { this->rasterization->setCommand(command); };
        if (this->rayTracing.has()) { this->rayTracing->setCommand(command); };
    };

    void RenderCommand::setDescriptorSets(vkt::uni_ptr<PipelineLayout> ilayout) {
        if (ilayout.has()) { this->layout = ilayout; }; 
        if (this->layout.has()) {
            if (this->rasterization.has()) { this->rasterization->setDescriptorSets(this->layout); };
            if (this->rayTracing.has()) { this->rayTracing->setDescriptorSets(this->layout); };
        };
    };

};
