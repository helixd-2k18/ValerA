#pragma once

// 
#include "./stdafx.h"
//#include "renderdoc_app.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vec_swizzle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

// This is wrapped library for OpenGL based applications
// TODO: Adding RT support for LibLava (unofficial and official)
// C++ sideway (static data only)
// C++ and C code can be mixed!
namespace vrc {
    inline static vlj::Driver driver = {};
    inline static uint32_t instanceCount = 0u;
    inline static uint32_t materialCount = 0u;

    // geometries
    inline static vlj::Framebuffer framebuffer = {};
    inline static vlj::PipelineLayout layout = {};
    inline static vlj::Background background = {};
    inline static vlj::Constants constants = {};
    inline static vlj::Rasterization rasterization = {};
    inline static vlj::RayTracing rayTracing = {};

    //
    inline static vlj::VertexSet vertexSet = {};
    inline static vlj::BufferViewSet bufferViews = {};
    inline static vlj::AttributeSet attributeSet = {};
    inline static vlj::BindingSet bindingSet = {};
    inline static std::vector<vlj::Geometry> geometries = {};

    // bodies
    inline static std::vector<vlj::GeometrySet> geometrySets = {};
    inline static std::vector<vlj::Acceleration> accelerations = {};

    // draw calls
    inline static vlj::InstanceSet instanceSet = {};
    inline static vlj::Acceleration accelerationTop = {};

    // materials
    inline static vlj::MaterialSet materialSet = {};
    inline static vlj::TextureSet textureSet = {};
    inline static vlj::SamplerSet samplerSet = {};

    // low-level data
    inline static std::vector<vkt::ImageRegion> images = {};
    inline static std::vector<vlj::SetBase> buffers = {};

    //
    inline static vlj::RenderCommand renderCommand = {};
    inline static vlj::BuildCommand buildCommand = {};


    // 
    class Slots { public: 
        uint32_t count = 0u;
        std::vector<int32_t> available = {};

        // 
        Slots() {};
        Slots(const Slots& slots) : count(slots.count), available(slots.available) {};
        Slots(const uint32_t& maxCount) {
            available.clear();
            available.resize(0u);
            for (uint32_t i = 0; i < maxCount; i++) {
                available.push_back(i);
            };
        };

        // 
        Slots& operator=(const Slots& slots) {
            this->count = slots.count, this->available = slots.available;
        };

        // 
        int32_t consume() {
            int32_t id = available[count];
            if (count < available.size()) {
                int32_t& idr = available[count++]; idr = -1;
            };
            return id;
        };

        // 
        void rise(const int32_t& used) {
            if (count > 0 && used != -1 && available[count-1] == -1) {
                available[--count] = used; 
            };
        };
    };

    //
    inline static Slots availableBuffers = {};
    inline static Slots availableImages = {};

    // data slots
    inline static Slots availableTextures = {};
    inline static Slots availableSamplers = {};
    inline static Slots availableMaterials = {};

    // geometry slots
    inline static Slots availableGeometries = {};
    inline static Slots availableGeometrySets = {};

    // per every geometry
    inline static const uint32_t accessorCount = 3, bindingsCount = 1;
    inline static const uint32_t maxInstances = 256;
    inline static const uint32_t maxGeometries = 256;


    // formats
    enum Format : uint32_t {
        FLOAT = 0,
        HALF = 1,
        INT = 2
    };

    // indices
    enum Index : uint32_t {
        NONE = 0,
        UINT8 = 1,
        UINT16 = 2,
        UINT32 = 3
    };

    // attributes
    struct AttribLayout {
        uint32_t offset = 0u;
        uint32_t count = 0u;
        Format format = Format::FLOAT;
    };

    // defaultly Neverball layout
    struct LayoutPreset {
        AttribLayout vertex = { .offset = 0u, .count = 3u, .format = Format::FLOAT };
        AttribLayout normal = { .offset = 12u, .count = 3u, .format = Format::FLOAT };
        AttribLayout texcoord = { .offset = 24u, .count = 3u, .format = Format::FLOAT };
        AttribLayout color = { .offset = 24u, .count = 0u, .format = Format::FLOAT };
    };

    // 
    class State { public:
        inline static uint32_t stride = 1u;
        inline static Index indexType = Index::NONE;
        inline static LayoutPreset preset = LayoutPreset{};
    };

    //
    void initVertexLayout(uint32_t stride, Index indexType, LayoutPreset preset) {
        State::preset = preset;
        State::stride = stride;
        State::indexType = indexType;
    };

    // 
    void encodeFormat(VkVertexInputAttributeDescription& desc, const AttribLayout& layout) {
        if (layout.format == Format::FLOAT) {
            if (layout.count == 4) { desc.format = VK_FORMAT_R32G32B32A32_SFLOAT; };
            if (layout.count == 3) { desc.format = VK_FORMAT_R32G32B32_SFLOAT; };
            if (layout.count == 2) { desc.format = VK_FORMAT_R32G32_SFLOAT; };
            if (layout.count == 1) { desc.format = VK_FORMAT_R32_SFLOAT; };
        };
        if (layout.format == Format::INT) {
            if (layout.count == 4) { desc.format = VK_FORMAT_R32G32B32A32_SINT; };
            if (layout.count == 3) { desc.format = VK_FORMAT_R32G32B32_SINT; };
            if (layout.count == 2) { desc.format = VK_FORMAT_R32G32_SINT; };
            if (layout.count == 1) { desc.format = VK_FORMAT_R32_SINT; };
        };
    };

    //
    int32_t createGeometrySet(std::vector<int32_t> geom = {}) {
        int32_t ptr = availableGeometrySets.consume();
        if (ptr >= geometrySets.size()) {
            geometrySets.resize(ptr + 1);
        };

        if (ptr >= 0) {
            std::vector<int64_t> primitiveCount = {};
            for (auto& gi : geom) {
                primitiveCount.push_back(geometries[gi].getDesc()->primitiveCount);
            };

            // 
            auto geometrySet = std::make_shared<vlr::GeometrySet>(vertexSet, vlr::DataSetCreateInfo{ .count = 1u });
            auto acceleration = std::make_shared<vlr::Acceleration>(driver, vlr::AccelerationCreateInfo{ .geometrySet = geometrySet, .initials = primitiveCount }); // Unknown Behaviour

            // 
            for (auto& gi : geom) {
                geometrySet->pushGeometry(geometries[gi]);
            };

            //
            geometrySets[ptr] = geometrySet;
            accelerations[ptr] = acceleration;
        };

        return ptr;
    };

    //
    int32_t createGeometry(vlj::SetBase vertexData, vlj::SetBase indexData, vlr::GeometryDesc desc) {
        int32_t ptr = availableGeometries.consume();
        if (ptr >= geometries.size()) {
            geometries.resize(ptr + 1);
        };

        if (ptr >= 0) {
            // Bindings
            bufferViews.get(ptr * 2 + 0) = vertexData.getGpuBuffer();
            bufferViews.get(ptr * 2 + 1) = indexData.getGpuBuffer();
            *bindingSet.get(ptr) = vkh::VkVertexInputBindingDescription{ .binding = ptr * 2 + 0, .stride = State::stride };

            // mapping indices
            if (State::indexType != Index::NONE) { desc.indexBufferView = ptr * 2 + 1; };
            if (State::indexType == Index::NONE) { desc.indexType = VK_INDEX_TYPE_NONE_KHR; };
            if (State::indexType == Index::UINT8) { desc.indexType = VK_INDEX_TYPE_UINT8_EXT; };
            if (State::indexType == Index::UINT16) { desc.indexType = VK_INDEX_TYPE_UINT16; };
            if (State::indexType == Index::UINT32) { desc.indexType = VK_INDEX_TYPE_UINT32; };

            // mapping attributes
            {   // Vertex 
                const uint32_t vidx = ptr * 4 + 0;
                *attributeSet.get(vidx) = vkh::VkVertexInputAttributeDescription{ .location = 0, .binding = ptr, .offset = State::preset.vertex.offset, };
                encodeFormat(*attributeSet.get(vidx), State::preset.vertex);
                desc.vertexAttribute = vidx;
            };

            {   // Texcoord
                const uint32_t vidx = ptr * 4 + 1;
                *attributeSet.get(vidx) = vkh::VkVertexInputAttributeDescription{ .location = 1, .binding = ptr, .offset = State::preset.texcoord.offset, };
                encodeFormat(*attributeSet.get(vidx), State::preset.texcoord);
                if (State::preset.texcoord.count != 0) { desc.mesh_flags.hasTexcoord = 1; };
                desc.attributes[0] = vidx;
            };

            {   // Normals
                const uint32_t vidx = ptr * 4 + 2;
                *attributeSet.get(vidx) = vkh::VkVertexInputAttributeDescription{ .location = 2, .binding = ptr, .offset = State::preset.normal.offset, };
                encodeFormat(*attributeSet.get(vidx), State::preset.normal);
                if (State::preset.normal.count != 0) { desc.mesh_flags.hasNormal = 1; };
                desc.attributes[1] = vidx;
            };

            // 
            geometries[ptr] = std::make_shared<vlr::Geometry>(vertexSet, desc);
        };

        return ptr;
    };

    //
    int32_t createGeometry(int32_t vertexData, int32_t indexData, vlr::GeometryDesc desc) {
        if (indexData < 0) { desc.indexType = VK_INDEX_TYPE_NONE_KHR; };
        return createGeometry(buffers[vertexData], indexData >= 0 ? buffers[indexData] : buffers[0u], desc);
    };

    // 
    int32_t createBuffer(VkDeviceSize count = 1u, bool uniform = false) {
        int32_t ptr = availableBuffers.consume();
        if (ptr >= buffers.size()) {
            buffers.resize(ptr + 1);
        };
        if (ptr >= 0) {
            buffers[ptr] = vlj::SetBase(driver, vlr::DataSetCreateInfo{
                .count = count,
                .uniform = uniform,
                .enableGL = true
            });
        };
        return ptr;
    };

    // MAY REQUIRE ADVANCED API
    int32_t createImage2D(const uint32_t& width = 2u, const uint32_t& height = 2, const VkFormat& format = VK_FORMAT_R8G8B8A8_UNORM, const uint32_t& levels = 1u) {
        int32_t ptr = availableImages.consume();
        if (ptr >= images.size()) {
            images.resize(ptr + 1);
        };

        // 
        if (ptr >= 0) {
            const auto aspect = vkh::VkImageAspectFlags{ .eColor = 1 };
            const auto apres = vkh::VkImageSubresourceRange{ .aspectMask = aspect };
            const auto imageUsage = vkh::VkImageUsageFlags{ .eTransferSrc = 1, .eTransferDst = 1, .eSampled = 1, .eStorage = 1, .eColorAttachment = 1 };

            // 
            vkt::VmaMemoryInfo memInfo = {};
            memInfo.memUsage = VMA_MEMORY_USAGE_GPU_ONLY;
            images[ptr] = vkt::ImageRegion(std::make_shared<vkt::VmaImageAllocation>(driver->getAllocator(), vkh::VkImageCreateInfo{}.also([=](vkh::VkImageCreateInfo* it) {
                it->format = format,
                it->extent = vkh::VkExtent3D{ uint32_t(width),uint32_t(height),1u },
                it->usage = imageUsage;
                return it;
            }), memInfo), vkh::VkImageViewCreateInfo{}.also([=](vkh::VkImageViewCreateInfo* it) {
                it->format = format,
                it->subresourceRange = apres;
                return it;
            }));
        };
        return ptr;
    };

    //
    void* mapBuffer(const int32_t& buffer) {
        return buffers[buffer].getCpuBuffer().mapv();
    };

    // 
    void copyBufferFromCpu(const int32_t& buffer) {
        driver->submitOnce([&](VkCommandBuffer& cmd) {
            auto pset = buffers[buffer];
            pset.setCommand(cmd);
        });
    };

    //
    void copyBufferToImage(const int32_t& buffer, const int32_t& image) {
        const uint32_t& width = images[image].getInfo().extent.width;
        const uint32_t& height = images[image].getInfo().extent.height;
        const auto& memInfo = images[image].getAllocation()->getAllocationInfo();

        //
        driver->submitOnce([&](VkCommandBuffer& cmd) {
            images[image].transfer(cmd);

            // 
            auto pset = buffers[buffer];
            driver->getDeviceDispatch()->CmdCopyBufferToImage(cmd, pset.getCpuBuffer().buffer(), images[image].getImage(), images[image].getImageLayout(), 1u, vkh::VkBufferImageCopy{
                .bufferOffset = pset.getCpuBuffer().offset(),
                .bufferRowLength = uint32_t(width),
                .bufferImageHeight = uint32_t(height),
                .imageSubresource = images[image].subresourceLayers(),
                .imageOffset = {0u,0u,0u},
                .imageExtent = {uint32_t(width),uint32_t(height),1u},
            });
        });
    };

    // 
    void initFramebuffer(const uint32_t& width, const uint32_t& height) {
        framebuffer.createFramebuffer(width, height);
        layout->setFramebuffer(framebuffer);
    };

    // 
    void initialize(const uint32_t& deviceID) {
        driver = std::make_shared<vlr::Driver>();
        auto instance = driver->createInstance();
        auto physicalDevice = driver->getPhysicalDevice(0u);
        auto device = driver->createDevice(true, "./", false);
        auto queue = driver->getQueue();
        auto commandPool = driver->getCommandPool();
        auto allocator = driver->getAllocator();

        // data slots
        availableTextures = Slots(256);
        availableSamplers = Slots(256);
        availableMaterials = Slots(256);

        // geometry slots
        availableGeometries = Slots(maxGeometries);
        availableGeometrySets = Slots(256);

        // 
        availableBuffers = Slots(1024);
        availableImages = Slots(1024);

        // 
        constants = std::make_shared<vlr::Constants>(driver, vlr::DataSetCreateInfo{ .count = 1u, .uniform = true });
        layout = std::make_shared<vlr::PipelineLayout>(driver);
        materialSet = std::make_shared<vlr::MaterialSet>(driver, vlr::DataSetCreateInfo{ .count = 256 });
        textureSet = std::make_shared<vlr::TextureSet>(driver);
        samplerSet = std::make_shared<vlr::SamplerSet>(driver);
        background = std::make_shared<vlr::Background>(driver);

        // 
        bindingSet = std::make_shared<vlr::BindingSet>(driver, vlr::DataSetCreateInfo{ .count = maxGeometries });
        attributeSet = std::make_shared<vlr::AttributeSet>(driver, vlr::DataSetCreateInfo{ .count = maxGeometries * 4u });
        bufferViews = std::make_shared<vlr::BufferViewSet>(driver);
        vertexSet = std::make_shared<vlr::VertexSet>(driver, vlr::VertexSetCreateInfo{
            .bindings = bindingSet,
            .attributes = attributeSet,
            .bufferViews = bufferViews
        });

        // 
        rasterization = std::make_shared<vlr::Rasterization>(driver, vlr::PipelineCreateInfo{
            .layout = layout,
            .framebuffer = framebuffer,
            .instanceSet = instanceSet,
            .constants = constants
        });

        // 
        rayTracing = std::make_shared<vlr::RayTracing>(driver, vlr::RayTracingCreateInfo{
            .layout = layout,
            .framebuffer = framebuffer,
            .accelerationTop = accelerationTop,
            .constants = constants
        });

        // make instanceSet
        instanceSet = std::make_shared<vlr::InstanceSet>(driver, vlr::DataSetCreateInfo{ .count = maxInstances });
        accelerationTop = std::make_shared<vlr::Acceleration>(driver, vlr::AccelerationCreateInfo{ .instanceSet = instanceSet, .initials = { maxInstances } }); // shapes.size()

        // 
        framebuffer = std::make_shared<vlr::Framebuffer>(driver);
        framebuffer.createRenderPass();

        // 
        layout->setBackground(background);
        layout->setMaterials(materialSet, textureSet, samplerSet);
        layout->setVertexSet(vertexSet);

        //
        renderCommand = std::make_shared<vlr::RenderCommand>(driver, vlr::RenderCommandCreateInfo{
            .layout = layout,
            .rayTracing = rayTracing,
            .rasterization = rasterization
        });

        // 
        buildCommand = std::make_shared<vlr::BuildCommand>(driver, vlr::BuildCommandCreateInfo{
            .layout = layout,
            .accelerationTop = accelerationTop,
        });

        // 
        rasterization->setDescriptorSets(layout);
        rayTracing->setDescriptorSets(layout);
        renderCommand->setDescriptorSets(layout);
        buildCommand->setDescriptorSets(layout);
    };
};

// implementation for C++ wrapper API (bridge between hardcode and softcode)
#include "./renderer.hpp"
namespace vrp {
#ifdef VLR_CPP_RENDERER
    void initialize(const uint32_t& deviceID) {
        vrc::initialize(deviceID);
    };

    void initFramebuffer(const uint32_t& width, const uint32_t& height) {
        vrc::initFramebuffer(width, height);
    };

    int32_t registerTexture(const Image2D& image) {
        int32_t ptr = vrc::availableTextures.consume();
        if (ptr >= 0) {
            vrc::textureSet.get() = vrc::images[image];
        };
        return ptr;
    };

    int32_t registerSampler(const VkSampler& sampler) {
        int32_t ptr = vrc::availableSamplers.consume();
        if (ptr >= 0) {
            vrc::samplerSet.get() = sampler;
        };
        return ptr;
    };

    int32_t registerMaterial(const vlr::MaterialUnit& materialUnit) {
        int32_t ptr = vrc::availableMaterials.consume();
        if (ptr >= 0) {
            *vrc::materialSet.get(ptr) = materialUnit;
        };
        return ptr;
    };

    vlr::ConstantDesc* editConstants() {
        return vrc::constants.get(0);
    };

    vlr::MaterialUnit* editMaterial(const int32_t& ptr) {
        if (ptr >= 0) { return vrc::materialSet.get(ptr); };
        return nullptr;
    };

    Image2D::Image2D(const uint32_t& width, const uint32_t& height, const VkFormat& format, const uint32_t& levels) {
        *this = vrc::createImage2D(width, height, format, levels);
    };

    BufferSet::BufferSet(const VkDeviceSize& count, const bool& uniform) {
        *this = vrc::createBuffer(count, uniform);
    };

    Geometry::Geometry(const BufferSet& vertexData, const BufferSet& indexData, const vlr::GeometryDesc& desc) {
        *this = vrc::createGeometry(vertexData, indexData, desc);
    };

    GeometrySet::GeometrySet(const std::vector<Geometry>& geoms = {}) {
        std::vector<int32_t> geomIds = {};
        for (auto& geom : geoms) { geomIds.push_back(geom); };
        *this = vrc::createGeometrySet(geomIds);
    };

    void GeometrySet::setCommand(const VkCommandBuffer& cmd) {
        vrc::geometrySets[this->ID]->setCommand(cmd);
        vrc::accelerations[this->ID]->setCommand(cmd);
    };

    void BufferSet::copyToImage(const Image2D& image) {
        vrc::copyBufferToImage(*this, image);
    };

    void BufferSet::copyFromCpu() {
        vrc::copyBufferFromCpu(*this);
    };

    void* BufferSet::map() {
        return vrc::mapBuffer(*this);
    };

    const void* BufferSet::map() const {
        return vrc::mapBuffer(*this);
    };

    //
    void drawObject(const GeometrySet& set) {
        vrc::instanceSet->get(vrc::instanceCount++) = vkh::VsGeometryInstance{
            .customId = uint32_t(set.ID),
            .accelerationStructureHandle = vrc::accelerations[set.ID]->getHandle() // Broken Feature, Set Manually
        };
    };

    // 
    const uint32_t FBufID = 4u;

    //
    void instanceCommand(const VkCommandBuffer& cmd) {
        vrc::instanceSet->setCommand(cmd, true);
        vrc::buildCommand->setCommandTop(cmd);
        vrc::instanceCount = 0u;
    };

    //
    void rayTraceCommand(const VkCommandBuffer& cmd) {
        vrc::materialSet->setCommand(cmd);
        vrc::constants->setCommand(cmd, true);
        vrc::renderCommand->setCommand(cmd);
        vrc::framebuffer->imageToLinearCopyCommand(cmd, FBufID);
        vrc::rayTracing->setCommandFinal(cmd);
    };

    //
    void geometryCommand(const VkCommandBuffer& cmd) {
        vrc::bindingSet->setCommand(cmd);
        vrc::attributeSet->setCommand(cmd);
        //vrc::buildCommand->setCommand(cmd); // don't build all geometries, only needed
    };
#endif
};
