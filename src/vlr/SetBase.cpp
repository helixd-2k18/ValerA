#include "./vlr/SetBase.hpp"
#include "./vlr/PipelineLayout.hpp"
#include "./vlr/Driver.hpp"

namespace vlr {

    void SetBase::createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) {
        this->descriptorSetInfo = vkh::VsDescriptorSetCreateInfoHelper(pipelineLayout->getSetLayout(), pipelineLayout->getDescriptorPool());
        auto& handle = this->descriptorSetInfo.pushDescription(vkh::VkDescriptorUpdateTemplateEntry{
            .dstBinding = 0u,
            .dstArrayElement = 0u,
            .descriptorCount = 1u,
            .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
        });
        handle.offset<VkDescriptorBufferInfo>(0) = this->gpuBuffer;
        vkh::handleVk(vkt::AllocateDescriptorSetWithUpdate(driver->getDevice(), this->descriptorSetInfo, this->set, this->updated));
    };
    
    void SetBase::setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier) {
        vkh::VkBufferCopy region = {};
        region.srcOffset = 0ull;
        region.dstOffset = 0ull;
        region.size = this->getGpuBuffer().range();
        driver->getDevice()->CmdCopyBuffer(this->getCpuBuffer(), this->getGpuBuffer(), 1, &region);
        if (barrier) { vkt::commandBarrier(driver->getDevice(), commandBuffer); }; // TODO: Advanced Barrier
    };

};
