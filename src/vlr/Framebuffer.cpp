#include "./vlr/Framebuffer.hpp"
#include "./vlr/PipelineLayout.hpp"
#include "./vlr/OptiXDenoise.hpp"

namespace vlr {

    // 
    void Framebuffer::constructor(vkt::uni_ptr<Driver> driver) {
        this->driver = driver;
        this->createRenderPass();
    };

    // 
    void Framebuffer::createRenderPass() {
        auto device = this->driver->getDeviceDispatch();

        //
        auto colAttachment = vkh::VkAttachmentDescription{
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_GENERAL,
            .finalLayout = VK_IMAGE_LAYOUT_GENERAL,
        };

        //
        auto depAttachment = vkh::VkAttachmentDescription{
            .format = VK_FORMAT_D32_SFLOAT_S8_UINT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        };

        //
        auto dep0 = vkh::VkSubpassDependency{ .srcSubpass = VK_SUBPASS_EXTERNAL, .dstSubpass = 0u };
        auto dep1 = vkh::VkSubpassDependency{ .srcSubpass = 0u, .dstSubpass = VK_SUBPASS_EXTERNAL };

        { // Fixing Linux Issue?
            auto srcStageMask = vkh::VkPipelineStageFlags{.eColorAttachmentOutput = 1, .eTransfer = 1, .eBottomOfPipe = 1}; ASSIGN(dep0, srcStageMask);
            auto dstStageMask = vkh::VkPipelineStageFlags{.eColorAttachmentOutput = 1};                                     ASSIGN(dep0, dstStageMask);
            auto srcAccessMask = vkh::VkAccessFlags{.eColorAttachmentWrite = 1};                                            ASSIGN(dep0, srcAccessMask);
            auto dstAccessMask = vkh::VkAccessFlags{.eColorAttachmentRead = 1, .eColorAttachmentWrite = 1};                 ASSIGN(dep0, dstAccessMask);
            auto dependencyFlags = vkh::VkDependencyFlags{.eByRegion = 1};                                                  ASSIGN(dep0, dependencyFlags);
        };

        { // Fixing Linux Issue?
            auto srcStageMask = vkh::VkPipelineStageFlags{.eColorAttachmentOutput = 1};                                     ASSIGN(dep1, srcStageMask);
            auto dstStageMask = vkh::VkPipelineStageFlags{.eTopOfPipe = 1, .eColorAttachmentOutput = 1, .eTransfer = 1};    ASSIGN(dep1, dstStageMask);
            auto srcAccessMask = vkh::VkAccessFlags{.eColorAttachmentRead = 1, .eColorAttachmentWrite = 1};                 ASSIGN(dep1, srcAccessMask);
            auto dstAccessMask = vkh::VkAccessFlags{.eColorAttachmentRead = 1, .eColorAttachmentWrite = 1};                 ASSIGN(dep1, dstAccessMask);
            auto dependencyFlags = vkh::VkDependencyFlags{.eByRegion = 1};                                                  ASSIGN(dep1, dependencyFlags);
        };

        {
            vkh::VsRenderPassCreateInfoHelper rpsInfo = {};
            for (uint32_t b = 0u; b < 8u; b++) { 
                rpsInfo.addColorAttachment(colAttachment);
                rasterFBO.blendStates.push_back(vkh::VkPipelineColorBlendAttachmentState{ .blendEnable = false });
                rasterFBO.clearValues.push_back(vkh::VkClearColorValue{}); rasterFBO.clearValues.back().color.float32 = glm::vec4(0.f, 0.f, 0.f, 0.f);
                resampleFBO.blendStates.push_back(vkh::VkPipelineColorBlendAttachmentState{ .blendEnable = true });
                resampleFBO.clearValues.push_back(vkh::VkClearColorValue{}); resampleFBO.clearValues.back().color.float32 = glm::vec4(0.f, 0.f, 0.f, 0.f);
            };

            // 
            rasterFBO.clearValues.push_back(vkh::VkClearColorValue{}); rasterFBO.clearValues.back().depthStencil = VkClearDepthStencilValue{ 1.0f, 0 };
            resampleFBO.clearValues.push_back(vkh::VkClearColorValue{}); resampleFBO.clearValues.back().depthStencil = VkClearDepthStencilValue{ 1.0f, 0 };

            // 
            rpsInfo.setDepthStencilAttachment(depAttachment);
            rpsInfo.addSubpassDependency(dep0);
            rpsInfo.addSubpassDependency(dep1);
            vkh::handleVk(device->CreateRenderPass(rpsInfo, nullptr, &rasterFBO.renderPass));
            vkh::handleVk(device->CreateRenderPass(rpsInfo, nullptr, &resampleFBO.renderPass));
        };
    };

    // 
    void Framebuffer::createFramebuffer(uint32_t width, uint32_t height) { //
        auto fbusage = vkh::VkImageUsageFlags{ .eTransferSrc = 1, .eTransferDst = 1, .eSampled = 1, .eStorage = 1, .eColorAttachment = 1 };
        auto lnusage = vkh::VkImageUsageFlags{ .eTransferSrc = 1, .eTransferDst = 1 };
        auto aspect = vkh::VkImageAspectFlags{.eColor = 1};
        auto apres = vkh::VkImageSubresourceRange{.aspectMask = aspect};
        auto device = this->driver->getDeviceDispatch();
        //auto allocInfo = this->driver->memoryAllocationInfo();
        auto allocator = this->driver->getAllocator();
        auto vmaMemInfo = vkt::VmaMemoryInfo{ .memUsage = VMA_MEMORY_USAGE_GPU_ONLY };


        // 
        std::vector<VkImageView> rasterAttachments = {};
        std::vector<VkImageView> resampleAttachments = {};

        {   //
            auto aspect = vkh::VkImageAspectFlags{.eDepth = 1, .eStencil = 1};
            auto dpuse = vkh::VkImageUsageFlags{.eTransferDst = 1, .eSampled = 1, .eDepthStencilAttachment = 1 };
            auto dpres = vkh::VkImageSubresourceRange{.aspectMask = aspect};
            dpuse = dpuse | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            
            //
            depthStencilImage = vkt::ImageRegion(std::make_shared<vkt::VmaImageAllocation>(allocator, vkh::VkImageCreateInfo{
                .format = VK_FORMAT_D32_SFLOAT_S8_UINT,
                .extent = {width,height,1u},
                .usage = dpuse,
            }, vmaMemInfo), vkh::VkImageViewCreateInfo{
                .format = VK_FORMAT_D32_SFLOAT_S8_UINT,
                .subresourceRange = dpres,
            }, VK_IMAGE_LAYOUT_GENERAL);

            // 
            vkt::submitOnce(this->driver->getDeviceDispatch(), this->driver->getQueue(), this->driver->getCommandPool(), [&,this](VkCommandBuffer& cmd) {
                this->driver->getDeviceDispatch()->CmdClearDepthStencilImage(cmd, depthStencilImage.transfer(cmd), depthStencilImage.getImageLayout(), vkh::VkClearDepthStencilValue{ .depth = 1.0f, .stencil = 0 }, 1u, depthStencilImage.getImageSubresourceRange());

                // 
                vkt::commandBarrier(this->driver->getDeviceDispatch(), cmd);
                vkt::imageBarrier(cmd, vkt::ImageBarrierInfo{
                    .image = depthStencilImage.getImage(),
                    .targetLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                    .originLayout = VK_IMAGE_LAYOUT_GENERAL,
                    .subresourceRange = vkh::VkImageSubresourceRange{ {}, 0u, 1u, 0u, 1u }.also([=](auto* it) {
                        auto aspect = vkh::VkImageAspectFlags{.eDepth = 1u, .eStencil = 1u };
                        it->aspectMask = aspect;
                        return it;
                    })
                });
                
            });
        };

        // 
        vkt::MemoryAllocationInfo almac = {};
        almac.memUsage = VMA_MEMORY_USAGE_GPU_ONLY;
        almac.glMemory = almac.glID = 0u;
        almac.queueFamilyIndices = {};
        almac.memoryProperties = driver->getMemoryProperties().memoryProperties;
        almac.instanceDispatch = driver->getInstanceDispatch();
        almac.deviceDispatch = driver->getDeviceDispatch();
        almac.instance = driver->getInstance();
        almac.device = driver->getDevice();

        // 
        auto createImage = [=,this](VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT, uint32_t wide = 1u) {
            auto image = vkt::ImageRegion(std::make_shared<vkt::ImageAllocation>(vkh::VkImageCreateInfo{
                .format = format,
                .extent = {width*wide,height,1u},
                .tiling = VK_IMAGE_TILING_OPTIMAL, // Required for RGBA32F and OptiX Interaction (Linear)
                .usage = fbusage,
            }, almac), vkh::VkImageViewCreateInfo{
                .format = format,
                .subresourceRange = apres
            }, VK_IMAGE_LAYOUT_GENERAL);

            // 
            vkt::submitOnce(this->driver->getDeviceDispatch(), this->driver->getQueue(), this->driver->getCommandPool(), [&,this](VkCommandBuffer& cmd) {
                image.transfer(cmd);
                this->driver->getDeviceDispatch()->CmdClearColorImage(cmd, image, image.getImageLayout(), vkh::VkClearColorValue{ .float32 = { 0.f,0.f,0.f,0.f } }, 1u, image.getImageSubresourceRange());
            });

            return image;
        };

        // 
        auto createImageLinear = [=, this](VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT, uint32_t wide = 1u) {
            auto image = vkt::ImageRegion(std::make_shared<vkt::ImageAllocation>(vkh::VkImageCreateInfo{
                .format = format,
                .extent = {width * wide,height,1u},
                .tiling = VK_IMAGE_TILING_LINEAR, // Required for RGBA32F and OptiX Interaction (Linear)
                .usage = lnusage,
            }, almac), vkh::VkImageViewCreateInfo{
                .format = format,
                .subresourceRange = apres
            }, VK_IMAGE_LAYOUT_GENERAL);

            // 
            vkt::submitOnce(this->driver->getDeviceDispatch(), this->driver->getQueue(), this->driver->getCommandPool(), [&, this](VkCommandBuffer& cmd) {
                image.transfer(cmd);
                this->driver->getDeviceDispatch()->CmdClearColorImage(cmd, image, image.getImageLayout(), vkh::VkClearColorValue{ .float32 = { 0.f,0.f,0.f,0.f } }, 1u, image.getImageSubresourceRange());
            });

            return image;
        };

        // 
        this->atomicMapping = createImage(VK_FORMAT_R32_UINT);

        // 
        auto createFramebuffer = [&,this](RenderPass& fbo, const std::vector<VkImageView>& attachments){
            vkh::handleVk(device->CreateFramebuffer(vkh::VkFramebufferCreateInfo{
                .renderPass = fbo.renderPass,
                .attachmentCount = static_cast<uint32_t>(attachments.size()),
                .pAttachments = attachments.data(),
                .width = width,
                .height = height
            }, nullptr, &fbo.framebuffer));
        };

        // 
        for (uint32_t b = 0u; b < 12u; b++) { // 
            currentsImages.push_back(createImage(VK_FORMAT_R32_SFLOAT, 4u)); // Required for GL_EXT_shader_atomic_float i.e. super-vec4 
            previousImages.push_back(createImage(VK_FORMAT_R32_SFLOAT, 4u)); // Required for GL_EXT_shader_atomic_float i.e. super-vec4 
        };

        // 
        for (uint32_t b = 0u; b < 12u; b++) {
            resampleImages.push_back(createImage(VK_FORMAT_R32_SFLOAT, 4u)); // Required for GL_EXT_shader_atomic_float i.e. super-vec4 
            rasterImages.push_back(createImage());

            if (b < 8u) {
                resampleAttachments.push_back(resampleImages.back());
                rasterAttachments.push_back(rasterImages.back());
            };
        };

        //
        for (uint32_t b = 0; b < 4u; b++) {
            //inoutLinearImages.push_back(createImageLinear(VK_FORMAT_R32_SFLOAT)); // Busy!
            inoutLinearImages.push_back(createImageLinear(VK_FORMAT_R32G32B32A32_SFLOAT)); // 4x free... 
        };

        // 
        {
            this->samplers.push_back(VK_NULL_HANDLE);
            device->CreateSampler(vkh::VkSamplerCreateInfo{
                .magFilter = VK_FILTER_LINEAR,
                .minFilter = VK_FILTER_LINEAR,
                .addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
                .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
                .unnormalizedCoordinates = true,
            }, nullptr, &this->samplers.back());

            this->samplers.push_back(VK_NULL_HANDLE);
            device->CreateSampler(vkh::VkSamplerCreateInfo{
                .magFilter = VK_FILTER_LINEAR,
                .minFilter = VK_FILTER_LINEAR,
                .addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
                .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            }, nullptr, &this->samplers.back());

            this->samplers.push_back(VK_NULL_HANDLE);
            device->CreateSampler(vkh::VkSamplerCreateInfo{
                .magFilter = VK_FILTER_LINEAR,
                .minFilter = VK_FILTER_LINEAR,
                .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
                .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            }, nullptr, &this->samplers.back());

            this->samplers.push_back(VK_NULL_HANDLE);
            device->CreateSampler(vkh::VkSamplerCreateInfo{
                .magFilter = VK_FILTER_LINEAR,
                .minFilter = VK_FILTER_LINEAR,
                .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
                .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            }, nullptr, &this->samplers.back());
        };

        // 
        resampleAttachments.push_back(depthStencilImage);
        rasterAttachments.push_back(depthStencilImage);

        // 
        //createFramebuffer(resampleFBO, resampleAttachments); // Using Atomic Float
        createFramebuffer(rasterFBO, rasterAttachments);

        // 
        scissor = vkh::VkRect2D{ vkh::VkOffset2D{0, 0}, vkh::VkExtent2D{width, height} };
        viewport = vkh::VkViewport{ 0.0f, static_cast<float>(scissor.extent.height), static_cast<float>(scissor.extent.width), -static_cast<float>(scissor.extent.height), 0.f, 1.f };
    };

    // 
    void Framebuffer::createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) {
        auto device = this->driver->getDeviceDispatch();
        auto descriptorSetInfo = vkh::VsDescriptorSetCreateInfoHelper(pipelineLayout->getFramebufferSetLayout(), pipelineLayout->getDescriptorPool());
        auto createDescriptorSetImages = [&,this](const std::vector<vkt::ImageRegion>& images, const uint32_t& binding = 0u, const VkDescriptorType& type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE)
        {   // 
            auto& handle = descriptorSetInfo.pushDescription(vkh::VkDescriptorUpdateTemplateEntry{
                .dstBinding = binding,
                .dstArrayElement = 0u,
                .descriptorCount = uint32_t(images.size()),
                .descriptorType = type
            });
            for (uintptr_t i = 0; i < images.size(); i++) {
                handle.offset<VkDescriptorImageInfo>(i) = images[i];
            };
        };

        {   // Samplers
            auto& handle = descriptorSetInfo.pushDescription(vkh::VkDescriptorUpdateTemplateEntry{
                .dstBinding = 4u,
                .dstArrayElement = 0u,
                .descriptorCount = uint32_t(samplers.size()),
                .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER
            });
            for (uintptr_t i = 0; i < samplers.size(); i++) {
                handle.offset<VkDescriptorImageInfo>(i)->sampler = samplers[i];
            };
        };

        {   // 
            auto& handle = descriptorSetInfo.pushDescription(vkh::VkDescriptorUpdateTemplateEntry{
                .dstBinding = 5u,
                .dstArrayElement = 0u,
                .descriptorCount = 1u,
                .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE
            });
            handle.offset<VkDescriptorImageInfo>(0u) = this->atomicMapping;
        };

        // 
        createDescriptorSetImages(currentsImages,0u);
        createDescriptorSetImages(previousImages,1u);
        createDescriptorSetImages(resampleImages,2u);
        createDescriptorSetImages(rasterImages, 3u);
        createDescriptorSetImages(rasterImages, 6u, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);

        // 
        vkh::handleVk(vkt::AllocateDescriptorSetWithUpdate(device, descriptorSetInfo, this->set, this->updated));
    };

};
