#include "./vlr/Implementation.hpp"
#include "./vlr/Background.hpp"
#include "./vlr/PipelineLayout.hpp"

namespace vlr {
    
    void Background::constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<vkt::ImageRegion> image) {
        this->driver = driver, this->background = image.ref();

        // 
        int width = 2u, height = 2u;
        std::vector<glm::vec4> gSkyColor = {
            glm::vec4(0.9f,0.98,0.999f, 1.f),
            glm::vec4(0.9f,0.98,0.999f, 1.f),
            glm::vec4(0.9f,0.98,0.999f, 1.f),
            glm::vec4(0.9f,0.98,0.999f, 1.f)
        };

        //
        const auto  imageUsage = vkh::VkImageUsageFlags{ .eTransferSrc = 1, .eTransferDst = 1, .eSampled = 1, .eStorage = 1, .eColorAttachment = 1 };
        const auto  bufferUsage = vkh::VkBufferUsageFlags{ .eTransferSrc = 1, .eTransferDst = 1, .eUniformTexelBuffer = 1, .eStorageTexelBuffer = 1, .eUniformBuffer = 1, .eStorageBuffer = 1, .eIndexBuffer = 1, .eVertexBuffer = 1, .eTransformFeedbackBuffer = 1 };
        const auto& uploadUsage = bufferUsage;

        //
        auto aspect = vkh::VkImageAspectFlags{ .eColor = 1 };
        auto apres = vkh::VkImageSubresourceRange{ .aspectMask = aspect };

        // 
        if (!(this->background.has() && (*image).has())) {   //
            vkt::VmaMemoryInfo memInfo = {};
            memInfo.memUsage = VMA_MEMORY_USAGE_GPU_ONLY;
            memInfo.deviceDispatch = driver->getDeviceDispatch().get_shared();
            memInfo.instanceDispatch = driver->getInstanceDispatch().get_shared();

            // 
            auto image = vkt::ImageRegion(std::make_shared<vkt::VmaImageAllocation>(driver->getAllocator(), vkh::VkImageCreateInfo{}.also([=](vkh::VkImageCreateInfo* it) {
                it->format = VK_FORMAT_R32G32B32A32_SFLOAT,
                it->extent = vkh::VkExtent3D{ uint32_t(width),uint32_t(height),1u },
                it->usage = imageUsage;
                return it;
            }), memInfo), vkh::VkImageViewCreateInfo{}.also([=](vkh::VkImageViewCreateInfo* it) {
                it->format = VK_FORMAT_R32G32B32A32_SFLOAT,
                it->subresourceRange = apres;
                return it;
            }));

            //
            vkt::Vector<> imageBuf = {};
            {
                memInfo.memUsage = VMA_MEMORY_USAGE_CPU_TO_GPU;
                imageBuf = vkt::Vector<>(std::make_shared<vkt::VmaBufferAllocation>(driver->getAllocator(), vkh::VkBufferCreateInfo{ // experimental: callify
                    .size = size_t(width) * size_t(height) * sizeof(glm::vec4), .usage = uploadUsage,
                }, memInfo));
                memcpy(imageBuf.data(), gSkyColor.data(), size_t(width) * size_t(height) * sizeof(glm::vec4));
            };

            //
            driver->submitOnce([&,this](VkCommandBuffer& cmd) {
                image.transfer(cmd);
                driver->getDeviceDispatch()->CmdCopyBufferToImage(cmd, imageBuf.buffer(), image.getImage(), image.getImageLayout(), 1u, vkh::VkBufferImageCopy{
                    .bufferOffset = imageBuf.offset(),
                    .bufferRowLength = uint32_t(width),
                    .bufferImageHeight = uint32_t(height),
                    .imageSubresource = image.subresourceLayers(),
                    .imageOffset = {0u,0u,0u},
                    .imageExtent = {uint32_t(width),uint32_t(height),1u},
                });
            });

            //
            this->background = image;
        };
    };

    void Background::createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) {
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};
        if (background.has()) {
            descriptorSetInfo = vkh::VsDescriptorSetCreateInfoHelper(pipelineLayout->getBackgroundSetLayout(), pipelineLayout->getDescriptorPool());
            auto& handle = descriptorSetInfo.pushDescription(vkh::VkDescriptorUpdateTemplateEntry{
                .dstBinding = 0u,
                .dstArrayElement = 0u,
                .descriptorCount = 1u,
                .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
            });
            handle.offset<VkDescriptorImageInfo>(0u) = background;
            vkh::handleVk(vkt::AllocateDescriptorSetWithUpdate(driver->getDeviceDispatch(), descriptorSetInfo, this->set, this->updated));
        };
    };


};
