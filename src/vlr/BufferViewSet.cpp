#include "./vlr/BufferViewSet.hpp"
#include "./vlr/PipelineLayout.hpp"
#include "./vlr/Driver.hpp"

namespace vlr {

    void BufferViewSet::createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) {
        //auto device = this->driver->getDeviceDispatch();
        this->descriptorSetInfo = vkh::VsDescriptorSetCreateInfoHelper(pipelineLayout->getBufferViewSetLayout(), pipelineLayout->getDescriptorPool());
        auto& handle = this->descriptorSetInfo.pushDescription(vkh::VkDescriptorUpdateTemplateEntry{
            .dstBinding = 0u,
            .dstArrayElement = 0u,
            .descriptorCount = uint32_t(buffers.size()),
            .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
        });
        for (uintptr_t i = 0; i < buffers.size(); i++) {
            handle.offset<VkDescriptorBufferInfo>(i) = this->buffers[i];
        };
        vkh::handleVk(vkt::AllocateDescriptorSetWithUpdate(driver->getDeviceDispatch(), this->descriptorSetInfo, this->set, this->updated));
    };

};
