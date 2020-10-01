#pragma once

#include <vlr/Implementation.hpp>
#include <vlr/BufferViewSet.hpp>
#include <vlr/PipelineLayout.hpp>

namespace vlr {

    void BufferViewSet::createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) {
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};
        if (buffers.size() > 0) {
            descriptorSetInfo = vkh::VsDescriptorSetCreateInfoHelper(this->texelBuffer ? pipelineLayout->getByteBufferViewSetLayout() : pipelineLayout->getBufferViewSetLayout(), pipelineLayout->getDescriptorPool());
            auto& handle = descriptorSetInfo.pushDescription(vkh::VkDescriptorUpdateTemplateEntry{
                .dstBinding = 0u,
                .dstArrayElement = 0u,
                .descriptorCount = uint32_t(buffers.size()),
                .descriptorType = this->texelBuffer ? VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER : VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
            });
            if (this->texelBuffer) {
                for (uint32_t i = 0; i < buffers.size(); i++) {
                    handle.offset<VkBufferView>(i) = this->buffers[i].createBufferView(VK_FORMAT_R8_UINT);
                };
            } else {
                for (uint32_t i = 0; i < buffers.size(); i++) {
                    handle.offset<VkDescriptorBufferInfo>(i) = this->buffers[i];
                };
            };
            
            vkh::handleVk(vkh::AllocateDescriptorSetWithUpdate(driver->getDeviceDispatch(), descriptorSetInfo, this->set, this->updated));
        };
    };

};
