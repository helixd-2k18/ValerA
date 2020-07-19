#include "./vlr/SamplerSet.hpp"
#include "./vlr/PipelineLayout.hpp"
#include "./vlr/Driver.hpp"

namespace vlr {

    void SamplerSet::createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) {
        this->descriptorSetInfo = vkh::VsDescriptorSetCreateInfoHelper(pipelineLayout->getBufferViewSetLayout(), pipelineLayout->getDescriptorPool());
        auto& handle = this->descriptorSetInfo.pushDescription(vkh::VkDescriptorUpdateTemplateEntry{
            .dstBinding = 0u,
            .dstArrayElement = 0u,
            .descriptorCount = uint32_t(samplers.size()),
            .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER
        });
        for (uintptr_t i = 0; i < samplers.size(); i++) {
            handle.offset<VkDescriptorImageInfo>(i)->sampler = this->samplers[i];
        };
        vkh::handleVk(vkt::AllocateDescriptorSetWithUpdate(driver->getDevice(), this->descriptorSetInfo, this->set, this->updated));
    };

};
