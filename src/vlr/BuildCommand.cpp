#include "./vlr/BuildCommand.hpp"
#include "./vlr/Framebuffer.hpp"
#include "./vlr/Acceleration.hpp"
#include "./vlr/BufferViewSet.hpp"
#include "./vlr/Constants.hpp"
#include "./vlr/InstanceSet.hpp"

namespace vlr {

    void BuildCommand::constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<BuildCommandCreateInfo> info) {
        this->driver = driver;
        if (info.has()) {
            this->layout = info->layout;
            this->accelerationTop = info->accelerationTop;
        };
    };

    void BuildCommand::setDescriptorSets(vkt::uni_ptr<PipelineLayout> ilayout) { // 
        if (layout.has()) { this->layout = ilayout; };
        this->layout->setAccelerationTop(this->accelerationTop);
        // TODO: Port from RayTracing Interpolations and Geometry Set bindings
    };

    void BuildCommand::setCommand(vkt::uni_arg<VkCommandBuffer> buildCommand, vkt::uni_arg<glm::uvec4> meta) {
        auto accelerations = this->accelerationTop->instanceSet->accelerations;
        if (accelerations.size() > 0) {
            for (uint32_t i = 0; i < accelerations.size(); i++) {
                accelerations[i]->setCommand(buildCommand);
            };
        };
        vkt::commandBarrier(this->driver->getDeviceDispatch(), buildCommand);

        // Use Handles of Descriptor Sets
        if (this->accelerationTop.has() && this->accelerationTop->instanceSet.has() && accelerations.size() > 0) {
            for (uint32_t i = 0; i < this->accelerationTop->instanceSet->getCpuBuffer().size(); i++) {
                this->accelerationTop->instanceSet->get(i).accelerationStructureHandle = accelerations[this->accelerationTop->instanceSet->get(i).customId]->getHandle();
            };
            this->accelerationTop->instanceSet->setCommand(buildCommand);
        };
        vkt::commandBarrier(this->driver->getDeviceDispatch(), buildCommand);
    };

    void BuildCommand::setCommandTop(vkt::uni_arg<VkCommandBuffer> buildCommand, vkt::uni_arg<glm::uvec4> meta) {
        // 
        if (this->accelerationTop.has()) { // 
            //vkt::commandBarrier(this->driver->getDeviceDispatch(), buildCommand);
            //this->accelerationTop->instanceSet->setCommand(buildCommand);
            this->accelerationTop->setCommand(buildCommand);
            vkt::commandBarrier(this->driver->getDeviceDispatch(), buildCommand);
        };
    };

};
