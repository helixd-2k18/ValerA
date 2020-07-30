#include "./vlr/Background.hpp"
#include "./vlr/PipelineLayout.hpp"

namespace vlr {
    
    void Background::createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) {
        if (background.has()) {
            this->descriptorSetInfo = vkh::VsDescriptorSetCreateInfoHelper(pipelineLayout->getBackgroundSetLayout(), pipelineLayout->getDescriptorPool());
            auto& handle = this->descriptorSetInfo.pushDescription(vkh::VkDescriptorUpdateTemplateEntry{
                .dstBinding = 0u,
                .dstArrayElement = 0u,
                .descriptorCount = 1u,
                .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
            });
            handle.offset<VkDescriptorImageInfo>(0u) = background;
            vkh::handleVk(vkt::AllocateDescriptorSetWithUpdate(driver->getDeviceDispatch(), this->descriptorSetInfo, this->set, this->updated));
        };
    };


};
