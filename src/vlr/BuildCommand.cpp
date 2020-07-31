#include "./vlr/BuildCommand.hpp"
#include "./vlr/Framebuffer.hpp"
#include "./vlr/Acceleration.hpp"
#include "./vlr/BufferViewSet.hpp"
#include "./vlr/Interpolation.hpp"
#include "./vlr/Constants.hpp"
#include "./vlr/InstanceSet.hpp"

namespace vlr {

    void BuildCommand::constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<BuildCommandCreateInfo> info) {
        this->driver = driver;
        if (info.has()) {
            this->layout = info->layout;
            this->accelerations = info->accelerations;
            this->accelerationTop = info->accelerationTop;
        };
    };

    void BuildCommand::setDescriptorSets(vkt::uni_ptr<PipelineLayout> ilayout) { // 
        if (layout.has()) { this->layout = ilayout; };
        this->layout->setAccelerationTop(this->accelerationTop);
        // TODO: Port from RayTracing Interpolations and Geometry Set bindings
    };

    void BuildCommand::setCommand(vkt::uni_arg<VkCommandBuffer> buildCommand, const glm::uvec4& meta) {
        if (this->accelerations.size() > 0) {
            for (uint32_t i=0;i<accelerations.size();i++) {
                this->accelerations[i]->setCommand(buildCommand);
            };
        };
        vkt::commandBarrier(this->driver->getDeviceDispatch(), buildCommand);

        // Use Handles of Descriptor Sets
        if (this->accelerationTop.has() && this->accelerationTop->instanceSet.has() && this->accelerations.size() > 0) {
            for (uint32_t i=0;i<this->accelerationTop->instanceSet->getCpuBuffer().size();i++) {
                this->accelerationTop->instanceSet->get(i).accelerationStructureHandle = this->accelerations[this->accelerationTop->instanceSet->get(i).customId]->getHandle();
            };
            this->accelerationTop->instanceSet->setCommand(buildCommand);
        };

        // 
        if (this->accelerationTop.has()) { // 
            this->accelerationTop->setCommand(buildCommand);
            vkt::commandBarrier(this->driver->getDeviceDispatch(), buildCommand);
        };
    };

};
