#include "./vlr/Driver.hpp"
#include "./vlr/PipelineLayout.hpp"

namespace vlr {

    void PipelineLayout::constructor(vkt::uni_ptr<Driver> driver) {
        this->driver = driver;

        // TODO: Inline Uniforms
        auto pipusage = vkh::VkShaderStageFlags{.eVertex = 1, .eGeometry = 1, .eFragment = 1, .eCompute = 1, .eRaygen = 1, .eAnyHit = 1, .eClosestHit = 1, .eMiss = 1 };
        auto indexedf = vkh::VkDescriptorBindingFlags{ .eUpdateAfterBind = 1, .eUpdateUnusedWhilePending = 1, .ePartiallyBound = 1 };
        auto device = this->driver->getDeviceDispatch();

        { // 1. Construct Buffers Set Descriptor Set Layout
            auto helper = vkh::VsDescriptorSetLayoutCreateInfoHelper{};
            helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 0u, .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, .descriptorCount = 1u, .stageFlags = pipusage }, indexedf);

            layouts.push_back({});
            vkh::handleVk(device->CreateDescriptorSetLayout(helper, nullptr, &layouts.back()));
        };

        { // 2. Construct Texture Set Descriptor Set Layout
            auto helper = vkh::VsDescriptorSetLayoutCreateInfoHelper{};
            helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 0u, .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, .descriptorCount = 256u, .stageFlags = pipusage }, indexedf);

            layouts.push_back({});
            vkh::handleVk(device->CreateDescriptorSetLayout(helper, nullptr, &layouts.back()));
        };

        { // 3. Construct Sampler Set Descriptor Set Layout
            auto helper = vkh::VsDescriptorSetLayoutCreateInfoHelper{};
            helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 0u, .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER, .descriptorCount = 256u, .stageFlags = pipusage }, indexedf);

            layouts.push_back({});
            vkh::handleVk(device->CreateDescriptorSetLayout(helper, nullptr, &layouts.back()));
        };

        { // 4. Construct Set of Buffers Descriptor Set Layout (when Buffer per Geometry, Node, Acceleration Structure, Instance, etc.)
            auto helper = vkh::VsDescriptorSetLayoutCreateInfoHelper{};
            helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 0u, .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, .descriptorCount = 256u, .stageFlags = pipusage }, indexedf);

            layouts.push_back({});
            vkh::handleVk(device->CreateDescriptorSetLayout(helper, nullptr, &layouts.back()));
        };

        { // 5. Construct Uniform Buffer (Set) for Unified Operations
            auto helper = vkh::VsDescriptorSetLayoutCreateInfoHelper{};
            helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 0u, .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = 1u, .stageFlags = pipusage }, indexedf);

            layouts.push_back({});
            vkh::handleVk(device->CreateDescriptorSetLayout(helper, nullptr, &layouts.back()));
        };

        { // 6. Construct Background Image Descriptor Layout
            auto helper = vkh::VsDescriptorSetLayoutCreateInfoHelper{};
            helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 0u, .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, .descriptorCount = 1u, .stageFlags = pipusage }, indexedf);

            layouts.push_back({});
            vkh::handleVk(device->CreateDescriptorSetLayout(helper, nullptr, &layouts.back()));
        };

        { // 7. Construct Framebuffer Sets Layout
            auto helper = vkh::VsDescriptorSetLayoutCreateInfoHelper{};
            helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 0u, .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, .descriptorCount = 12u, .stageFlags = pipusage }, indexedf); // Previous Frame
            helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 1u, .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, .descriptorCount = 12u, .stageFlags = pipusage }, indexedf); // Currents Frame
            helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 2u, .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, .descriptorCount =  8u, .stageFlags = pipusage }, indexedf); // Resampled
            helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 3u, .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, .descriptorCount =  8u, .stageFlags = pipusage }, indexedf); // Rasterized 
            helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 4u, .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER      , .descriptorCount =  8u, .stageFlags = pipusage }, indexedf);

            layouts.push_back({});
            vkh::handleVk(device->CreateDescriptorSetLayout(helper, nullptr, &layouts.back()));
        };

        { // 8. Acceleration Structure Layout
            auto helper = vkh::VsDescriptorSetLayoutCreateInfoHelper{};
            helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 0u, .descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, .descriptorCount = 1u, .stageFlags = pipusage }, indexedf);

            layouts.push_back({});
            vkh::handleVk(device->CreateDescriptorSetLayout(helper, nullptr, &layouts.back()));
        };

        //
        auto pipusage = vkh::VkShaderStageFlags{.eVertex = 1, .eGeometry = 1, .eFragment = 1, .eCompute = 1, .eRaygen = 1, .eAnyHit = 1, .eClosestHit = 1, .eMiss = 1 };
        auto indexedf = vkh::VkDescriptorBindingFlags{ .eUpdateAfterBind = 1, .eUpdateUnusedWhilePending = 1, .ePartiallyBound = 1 };

        // 
        std::vector<VkDescriptorSetLayout> layouts = {};
        layouts.push_back(this->getUniformSetLayout());     // Firstly always constants
        layouts.push_back(this->getBufferViewSetLayout());  // Vertex Data buffers
        layouts.push_back(this->getSetLayout());            // Bindings
        layouts.push_back(this->getSetLayout());            // Attributes (Accessors)
        layouts.push_back(this->getFramebufferSetLayout()); // Framebuffers (666 in 1)
        layouts.push_back(this->getSetLayout());            // Materials
        layouts.push_back(this->getTextureSetLayout());     // Textures
        layouts.push_back(this->getSamplerSetLayout());     // Samplers
        layouts.push_back(this->getBufferViewSetLayout());  // Multiple Geometries Data
        layouts.push_back(this->getBufferViewSetLayout());  // Interpolation Attributes
        layouts.push_back(this->getAccelerationSetLayout());// Acceleration Structure
        layouts.push_back(this->getSetLayout());            // Counters
        layouts.push_back(this->getSetLayout());            // Ray Data

        // 
        this->bound.resize(layouts.size());

        // 
        std::vector<vkh::VkPushConstantRange> ranges = { vkh::VkPushConstantRange{.stageFlags = (stages = pipusage), .offset = 0u, .size = 16u } };
        vkh::handleVk(this->driver->getDeviceDispatch()->CreatePipelineLayout(vkh::VkPipelineLayoutCreateInfo{}.setSetLayouts(layouts).setPushConstantRanges(ranges), nullptr, &this->pipelineLayout));
    };

};
