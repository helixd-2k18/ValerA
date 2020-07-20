#include "./vlr/TextureSet.hpp"
#include "./vlr/PipelineLayout.hpp"
#include "./vlr/Driver.hpp"

namespace vlr {

    void TextureSet::createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) {
        this->descriptorSetInfo = vkh::VsDescriptorSetCreateInfoHelper(pipelineLayout->getBufferViewSetLayout(), pipelineLayout->getDescriptorPool());
        auto& handle = this->descriptorSetInfo.pushDescription(vkh::VkDescriptorUpdateTemplateEntry{
            .dstBinding = 0u,
            .dstArrayElement = 0u,
            .descriptorCount = uint32_t(images.size()),
            .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
        });
        for (uintptr_t i = 0; i < images.size(); i++) {
            handle.offset<VkDescriptorImageInfo>(i) = this->images[i];
        };
        vkh::handleVk(vkt::AllocateDescriptorSetWithUpdate(driver->getDeviceDispatch(), this->descriptorSetInfo, this->set, this->updated));
    };

};
