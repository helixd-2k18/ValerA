#include "./vlr/Driver.hpp"
#include "./vlr/PipelineLayout.hpp"

// 
#include "./BufferViewSet.hpp"
#include "./MaterialSet.hpp"
#include "./TextureSet.hpp"
#include "./SamplerSet.hpp"

//
#include "./VertexSet.hpp"
#include "./Framebuffer.hpp"

// 
#include "./Constants.hpp"
#include "./Acceleration.hpp"

// 
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
            helper.pushBinding(vkh::VkDescriptorSetLayoutBinding{ .binding = 5u, .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, .descriptorCount =  1u, .stageFlags = pipusage }, indexedf); // Atomic Mapping

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
        layouts.push_back(this->getUniformSetLayout());     // [ ] Firstly always constants
        layouts.push_back(this->getBufferViewSetLayout());  // [x] Vertex Data buffers
        layouts.push_back(this->getSetLayout());            // [x] Bindings
        layouts.push_back(this->getSetLayout());            // [x] Attributes (Accessors)
        layouts.push_back(this->getFramebufferSetLayout()); // [x] Framebuffers (666 in 1)
        layouts.push_back(this->getSetLayout());            // [x] Materials
        layouts.push_back(this->getTextureSetLayout());     // [x] Textures 
        layouts.push_back(this->getSamplerSetLayout());     // [x] Samplers 
        layouts.push_back(this->getBufferViewSetLayout());  // [x] Multiple Geometries Data 
        layouts.push_back(this->getBufferViewSetLayout());  // [x] Interpolation Attributes 
        layouts.push_back(this->getAccelerationSetLayout());// [x] Acceleration Structure
        layouts.push_back(this->getSetLayout());            // [x] Counters
        layouts.push_back(this->getBufferViewSetLayout());  // [x] Ray Data (FLIP)
        layouts.push_back(this->getSetLayout());            // [x] Hit Data
        layouts.push_back(this->getSetLayout());            // [x] Color-chain Data
        layouts.push_back(this->getBufferViewSetLayout());  // [x] Per-Mesh (Geometry) Attribute Indices
        layouts.push_back(this->getTextureSetLayout());     // [x] Background
        layouts.push_back(this->getSetLayout());            // [x] Instance Data

        // 
        this->bound.resize(layouts.size());

        // 
        std::vector<vkh::VkPushConstantRange> ranges = { vkh::VkPushConstantRange{.stageFlags = (stages = pipusage), .offset = 0u, .size = 16u } };
        vkh::handleVk(this->driver->getDeviceDispatch()->CreatePipelineLayout(vkh::VkPipelineLayoutCreateInfo{}.setSetLayouts(layouts).setPushConstantRanges(ranges), nullptr, &this->pipelineLayout));
    };

    void PipelineLayout::setMaterials(vkt::uni_ptr<MaterialSet> materialSet, vkt::uni_ptr<TextureSet> textureSet, vkt::uni_ptr<SamplerSet> samplerSet) {
        if (!materialSet->set) { materialSet->createDescriptorSet(this); };
        if (!textureSet->set) { textureSet->createDescriptorSet(this); };
        if (!samplerSet->set) { samplerSet->createDescriptorSet(this); };

        this->bound[5u] = materialSet->set;
        this->bound[6u] = textureSet->set;
        this->bound[7u] = samplerSet->set;
    };

    void PipelineLayout::setVertexData(vkt::uni_ptr<VertexSet> vertexData) {
        if (!vertexData->bufferViews->set) { vertexData->bufferViews->createDescriptorSet(this); };
        if (!vertexData->bindings->set) { vertexData->bindings->createDescriptorSet(this); };
        if (!vertexData->attributes->set) { vertexData->attributes->createDescriptorSet(this); };

        this->bound[1u] = vertexData->bufferViews->set;
        this->bound[2u] = vertexData->bindings->set;
        this->bound[3u] = vertexData->attributes->set;
    };

    void PipelineLayout::setFramebuffer(vkt::uni_ptr<Framebuffer> framebuffer) {
        if (!framebuffer->set) { framebuffer->createDescriptorSet(this); };

        this->bound[4u] = framebuffer->set;
    };

    void PipelineLayout::setAccelerationTop(vkt::uni_ptr<Acceleration> acceleration) {
        if (acceleration.has() && !acceleration->set) {
            acceleration->createDescriptorSet(this);
            if (acceleration->instanceSet.has() && !acceleration->instanceSet->set) {
                acceleration->instanceSet->createDescriptorSet(this);
            };
        };

        // 
        if (acceleration.has()) {
            this->bound[10u] = acceleration->set;
            if (acceleration->instanceSet.has()) {
                this->bound[17u] = acceleration->instanceSet->set;
            };
        };
    };

    void PipelineLayout::setConstants(vkt::uni_ptr<Constants> constants) { // 
        if (constants.has()) {
            if (!constants->set) {
                constants->createDescriptorSet(this);
            };
            this->bound[0u] = constants->set;
        };
    };

};
