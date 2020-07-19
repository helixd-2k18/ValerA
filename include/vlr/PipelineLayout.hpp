#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

    class PipelineLayout : public std::enable_shared_from_this<PipelineLayout> { protected: 
        VkPipelineCache cache = {};
        VkPipelineLayout bindings = {}, transform = {}; // Unified Bindings, Transform Feedback 
        VkDescriptorPool pool = {};
        
        // 
        std::vector<VkDescriptorSetLayout> layouts = {};
        std::shared_ptr<Driver> driver = {};
        
    public: 
        PipelineLayout() { this->constructor(); };
        PipelineLayout(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        // 
        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            this->driver = driver;

            // TODO: Inline Uniforms
            auto pipusage = vkh::VkShaderStageFlags{.eVertex = 1, .eGeometry = 1, .eFragment = 1, .eCompute = 1, .eRaygen = 1, .eAnyHit = 1, .eClosestHit = 1, .eMiss = 1 };
            auto indexedf = vkh::VkDescriptorBindingFlags{ .eUpdateAfterBind = 1, .eUpdateUnusedWhilePending = 1, .ePartiallyBound = 1 };

            { // 1. Construct Buffers Set Descriptor Set Layout
                auto helper = vkh::VsDescriptorSetLayoutCreateInfoHelper{};
                helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 0u, .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, .descriptorCount = 1u, .stageFlags = pipusage }, indexedf);

                layouts.push_back({});
                vkh::handleVk(this->driver->getDevice()->CreateDescriptorSetLayout(helper, nullptr, &layouts.back()));
            };

            { // 2. Construct Texture Set Descriptor Set Layout
                auto helper = vkh::VsDescriptorSetLayoutCreateInfoHelper{};
                helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 0u, .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, .descriptorCount = 256u, .stageFlags = pipusage }, indexedf);

                layouts.push_back({});
                vkh::handleVk(this->driver->getDevice()->CreateDescriptorSetLayout(helper, nullptr, &layouts.back()));
            };

            { // 3. Construct Sampler Set Descriptor Set Layout
                auto helper = vkh::VsDescriptorSetLayoutCreateInfoHelper{};
                helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 0u, .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER, .descriptorCount = 256u, .stageFlags = pipusage }, indexedf);

                layouts.push_back({});
                vkh::handleVk(this->driver->getDevice()->CreateDescriptorSetLayout(helper, nullptr, &layouts.back()));
            };

            { // 4. Construct Set of Buffers Descriptor Set Layout (when Buffer per Geometry, Node, Acceleration Structure, Instance, etc.)
                auto helper = vkh::VsDescriptorSetLayoutCreateInfoHelper{};
                helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 0u, .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, .descriptorCount = 256u, .stageFlags = pipusage }, indexedf);

                layouts.push_back({});
                vkh::handleVk(this->driver->getDevice()->CreateDescriptorSetLayout(helper, nullptr, &layouts.back()));
            };

            { // 5. Construct Uniform Buffer (Set) for Unified Operations
                auto helper = vkh::VsDescriptorSetLayoutCreateInfoHelper{};
                helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 0u, .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = 1u, .stageFlags = pipusage }, indexedf);

                layouts.push_back({});
                vkh::handleVk(this->driver->getDevice()->CreateDescriptorSetLayout(helper, nullptr, &layouts.back()));
            };

            { // 6. Construct Background Image Descriptor Layout
                auto helper = vkh::VsDescriptorSetLayoutCreateInfoHelper{};
                helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 0u, .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, .descriptorCount = 1u, .stageFlags = pipusage }, indexedf);
                helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 1u, .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER      , .descriptorCount = 8u, .stageFlags = pipusage }, indexedf);

                layouts.push_back({});
                vkh::handleVk(this->driver->getDevice()->CreateDescriptorSetLayout(helper, nullptr, &layouts.back()));
            };

            { // 7. Construct Framebuffer Sets Layout
                auto helper = vkh::VsDescriptorSetLayoutCreateInfoHelper{};
                helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 0u, .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, .descriptorCount = 12u, .stageFlags = pipusage }, indexedf); // Previous Frame
                helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 1u, .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, .descriptorCount = 12u, .stageFlags = pipusage }, indexedf); // Currents Frame
                helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 2u, .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, .descriptorCount =  8u, .stageFlags = pipusage }, indexedf); // Resampled
                helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 3u, .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, .descriptorCount =  8u, .stageFlags = pipusage }, indexedf); // Rasterized 
                
                layouts.push_back({});
                vkh::handleVk(this->driver->getDevice()->CreateDescriptorSetLayout(helper, nullptr, &layouts.back()));
            };
        };

        // 
        VkPipelineCache getPipelineCache() const { return cache; };
        VkPipelineLayout getBindingPipelineLayout() const { return bindings; };
        VkPipelineLayout getTransformPipelineLayout() const { return transform; };
        VkDescriptorSetLayout getDescriptorSetLayout(const uint32_t& I) const { return layouts[I]; };
        VkDescriptorSetLayout getSetLayout() const { return layouts[0u]; };
        VkDescriptorSetLayout getDescriptorPool() const { return layouts[3u]; };
        VkDescriptorSetLayout getTextureSetLayout() const { return layouts[1u]; };
        VkDescriptorSetLayout getSamplerSetLayout() const { return layouts[2u]; };
        VkDescriptorSetLayout getUniformSetLayout() const { return layouts[4u]; };
        VkDescriptorSetLayout getBufferViewSetLayout() const { return layouts[3u]; };
        VkDescriptorSetLayout getBackgroundSetLayout() const { return layouts[5u]; };
        VkDescriptorSetLayout getFramebufferSetLayout() const { return layouts[6u]; };
    };

};
