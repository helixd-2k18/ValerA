#include "./vlr/Implementation.hpp"
#include "./vlr/Acceleration.hpp"
#include "./vlr/PipelineLayout.hpp"
#include "./vlr/GeometrySet.hpp"
#include "./vlr/InstanceSet.hpp"
#include "./vlr/Geometry.hpp"

namespace vlr {

    // 
    void Acceleration::createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) {
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};
        descriptorSetInfo = vkh::VsDescriptorSetCreateInfoHelper(pipelineLayout->getAccelerationSetLayout(), pipelineLayout->getDescriptorPool());
        auto& handle = descriptorSetInfo.pushDescription(vkh::VkDescriptorUpdateTemplateEntry{
            .dstBinding = 0u,
            .dstArrayElement = 0u,
            .descriptorCount = 1u,
            .descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR
        });
        handle.offset<VkAccelerationStructureKHR>(0) = this->structure;
        vkh::handleVk(vkt::AllocateDescriptorSetWithUpdate(driver->getDeviceDispatch(), descriptorSetInfo, this->set, this->updated));
    };

    // Made implicit due some not-valid GLTF (disable overlap formats)
    uint32_t formatStride(const VkFormat& format) {
        // F32_T
        if (format >= 107 && format <= 109) { return sizeof(glm::vec4); } else
        if (format >= 104 && format <= 106) { return sizeof(glm::vec3); } else 
        if (format >= 101 && format <= 103) { return sizeof(glm::vec2); } else 
        if (format >= 98  && format <= 100) { return sizeof(float); }

        // F16_T
        // TODO: 

        return sizeof(uint32_t);
    };

    uint32_t predicate(const uint32_t& stride, const uint32_t& format) {
        return stride > 0u ? stride : format;
    };

    // 
    void Acceleration::updateAccelerationStructure(vkt::uni_arg<AccelerationCreateInfo> info, const bool& build) {
        offsetInfo.clear(); offsetInfo.resize(0u);
        buildGInfo.clear(); buildGInfo.resize(0u);
        offsetPtr.clear(); offsetPtr.resize(0u);
        buildGPtr.clear(); buildGPtr.resize(0u);

        // For monopolar Turing architecture
        auto maxGeometryCount = this->dataCreate[initialID].maxPrimitiveCount;

        // 
        if (info.has()) {
            this->geometrySet = info->geometrySet;
            this->instanceSet = info->instanceSet;
        };

        // 
        if (instanceSet.has()) {
            uint32_t i = 0u;
            {   // 
                auto offsetDesc = vkh::VkAccelerationStructureBuildOffsetInfoKHR{};
                auto buildGeometryFlags = vkh::VkGeometryFlagsKHR{ .eNoDuplicateAnyHitInvocation = 1 };
                auto geometryDesc = vkh::VkAccelerationStructureGeometryKHR{ .flags = buildGeometryFlags };
                auto instanceDesc = vkh::VkAccelerationStructureGeometryInstancesDataKHR{};
                auto gpuBuffer = instanceSet->getGpuBuffer();

                // 
                //std::cout << "Instance Sizeof = " << sizeof(vkh::VsGeometryInstance) << std::endl;
                instanceDesc.data = gpuBuffer->deviceAddress();
                instanceDesc.arrayOfPointers = false;
                geometryDesc.geometry.instances = instanceDesc;
                geometryDesc.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;

                // 
                offsetDesc.primitiveCount = std::min(uint32_t(std::min(gpuBuffer.size(), this->instanceCount)), (maxGeometryCount = this->dataCreate[initialID + i].maxPrimitiveCount));
                offsetDesc.transformOffset = 0u;
                offsetDesc.primitiveOffset = gpuBuffer.offset();
                offsetDesc.firstVertex = 0u;

                // 
                buildGInfo.push_back(geometryDesc);
                offsetInfo.push_back(offsetDesc);
            };
        } else
        if (geometrySet.has()) {
            // Generate Building Info
            const size_t geometryCount = std::min(this->dataCreate.size() - 1, geometrySet->geometries.size());
            for (uint32_t i=0u;i< geometryCount;i++)
            {   // 
                auto& geometry = geometrySet->geometries[i];
                auto& objectDesc = (geometrySet->get(i) = geometry->desc);
                auto& attribute = geometry->vertexSet->getAttribute(objectDesc.vertexAttribute);
                auto& binding = geometry->vertexSet->getBinding(attribute.binding);
                auto offsetDesc = vkh::VkAccelerationStructureBuildOffsetInfoKHR{};
                auto buildGeometryFlags = vkh::VkGeometryFlagsKHR{ .eOpaque = (objectDesc.mesh_flags.translucent ? 0u : 1u), .eNoDuplicateAnyHitInvocation = 1 };
                auto geometryDesc = vkh::VkAccelerationStructureGeometryKHR{ .flags = buildGeometryFlags };
                auto triangleDesc = vkh::VkAccelerationStructureGeometryTrianglesDataKHR{};
                auto buffer = geometry->vertexSet->getAttributeBuffer(objectDesc.vertexAttribute);

                // 
                triangleDesc.transformData = geometrySet->getGpuBuffer()->deviceAddress();
                triangleDesc.vertexFormat = attribute.format;
                triangleDesc.vertexStride = predicate(binding.stride, formatStride(triangleDesc.vertexFormat));
                triangleDesc.vertexData = buffer->deviceAddress();

                // 
                if (objectDesc.indexBufferView != ~0u && objectDesc.indexBufferView != -1 && objectDesc.indexType != VK_INDEX_TYPE_NONE_KHR) {
                    triangleDesc.indexData = geometry->vertexSet->getBuffer(objectDesc.indexBufferView)->deviceAddress();
                    triangleDesc.indexType = VkIndexType(objectDesc.indexType);
                } else {
                    triangleDesc.indexType = VK_INDEX_TYPE_NONE_KHR;
                    triangleDesc.indexData = VK_NULL_HANDLE;
                };

                // Fix vec4 formats into vec3, without alpha (but still can be passed by stride value)
                if (triangleDesc.vertexFormat == VK_FORMAT_R32G32B32A32_SFLOAT) { triangleDesc.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT; triangleDesc.vertexStride = std::max(triangleDesc.vertexStride, sizeof(glm::vec4)); };
                if (triangleDesc.vertexFormat == VK_FORMAT_R16G16B16A16_SFLOAT) { triangleDesc.vertexFormat = VK_FORMAT_R16G16B16_SFLOAT; triangleDesc.vertexStride = std::max(triangleDesc.vertexStride, sizeof(glm::u16vec4)); };
                geometryDesc.geometry.triangles = triangleDesc;
                geometryDesc.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;

                // 
                offsetDesc.firstVertex = objectDesc.firstVertex;
                offsetDesc.primitiveCount = std::min(objectDesc.primitiveCount, maxGeometryCount = this->dataCreate[initialID + i].maxPrimitiveCount);
                offsetDesc.transformOffset = i * sizeof(GeometryDesc);

                // Shifting...
                // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkAccelerationStructureBuildOffsetInfoKHR.html
                if (triangleDesc.indexType != VK_INDEX_TYPE_NONE_KHR) {
                    offsetDesc.firstVertex += buffer.offset() / triangleDesc.vertexStride;
                    offsetDesc.primitiveOffset += geometry->vertexSet->getBuffer(objectDesc.indexBufferView).offset();
                } else {
                    offsetDesc.primitiveOffset += buffer.offset(); // Vertex Anti-Bug-Tale
                };

                // 
                buildGInfo.push_back(geometryDesc);
                offsetInfo.push_back(offsetDesc);
            };
        };

        // 
        for (uint32_t i = 0u; i < buildGInfo.size(); i++) {
            buildGPtr.push_back(&buildGInfo[i]);
            offsetPtr.push_back(&offsetInfo[i]);
        };

        // FOR BUILD!
        auto bdHeadFlags = vkh::VkBuildAccelerationStructureFlagsKHR{ .eAllowUpdate = 1, .ePreferFastTrace = 1 };
        this->bdHeadInfo = vkh::VkAccelerationStructureBuildGeometryInfoKHR{};
        this->bdHeadInfo.geometryCount = uint32_t(buildGPtr.size());
        this->bdHeadInfo.ppGeometries = buildGPtr.data();
        this->bdHeadInfo.type = this->create.type;
        this->bdHeadInfo.flags = this->create.flags;
        this->bdHeadInfo.geometryArrayOfPointers = true;
        this->bdHeadInfo.dstAccelerationStructure = this->structure;
        this->bdHeadInfo.srcAccelerationStructure = VK_NULL_HANDLE;
        this->bdHeadInfo.update = false;
        this->bdHeadInfo.scratchData = this->gpuScratchBuffer.deviceAddress();

        // Only For Supported GPU's
        if (build) {
            vkh::handleVk(driver->getDeviceDispatch()->BuildAccelerationStructureKHR(1u, this->bdHeadInfo, reinterpret_cast<VkAccelerationStructureBuildOffsetInfoKHR**>(this->offsetPtr.data())));
        };
    };

    // 
    void Acceleration::constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<AccelerationCreateInfo> info) {
        this->driver = driver, this->instanceCount = info->maxInstanceCount, this->initialID = info->initialID;
        if (info.has()) {
            this->geometrySet = info->geometrySet;
            this->instanceSet = info->instanceSet;
        };
        auto device = this->driver->getDeviceDispatch();

        // NVIDIA Turing has broken AS creation, so needs additional element (i.e. crutch)
        if (!instanceSet.has()) {
            // Native bug-fix for Turing
            info->initials.push_back(1ll);
        };

        // FOR CREATE!
        this->dataCreate.resize(info->initials.size()); uintptr_t I = 0ull;

        // 
        for (auto& BC : this->dataCreate) {
            BC = vkh::VkAccelerationStructureCreateGeometryTypeInfoKHR{};
            if (instanceSet.has()) {
                BC.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
                BC.maxVertexCount = 0u;
                BC.maxPrimitiveCount = static_cast<uint32_t>(info->initials[I]);
                BC.indexType = VK_INDEX_TYPE_NONE_KHR;
                BC.vertexFormat = VK_FORMAT_UNDEFINED;
                BC.allowsTransforms = false;
            } else 
            {
                BC.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
                BC.maxVertexCount = static_cast<uint32_t>(info->initials[I] * 3u);
                BC.maxPrimitiveCount = static_cast<uint32_t>(info->initials[I]);
                BC.indexType = VK_INDEX_TYPE_NONE_KHR;
                BC.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
                BC.allowsTransforms = true;
            };
            I++;
        };

        // FOR CREATE! 
        {
            auto bdHeadFlags = vkh::VkBuildAccelerationStructureFlagsKHR{ .eAllowUpdate = 1, .ePreferFastTrace = 1 };
            this->create.maxGeometryCount = uint32_t(this->dataCreate.size());
            this->create.pGeometryInfos = this->dataCreate.data();
            this->create.type = instanceSet.has() ? VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR : VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
            this->create.flags = bdHeadFlags;
        };

        // 
        if (!this->structure) { // create acceleration structure fastly...
            vkh::handleVk(device->CreateAccelerationStructureKHR(this->create, nullptr, &this->structure));

            // 
            vkh::VkMemoryRequirements2 requirements = {};
            device->GetAccelerationStructureMemoryRequirementsKHR(vkh::VkAccelerationStructureMemoryRequirementsInfoKHR{
                .type = VK_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_TYPE_OBJECT_KHR,
                .buildType = VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
                .accelerationStructure = this->structure
            }, requirements);

            // TODO: fix memoryProperties issue
            auto usage = vkh::VkBufferUsageFlags{.eTransferDst = 1, .eStorageTexelBuffer = 1, .eStorageBuffer = 1, .eVertexBuffer = 1, .eSharedDeviceAddress = 1 };
            TempBuffer = vkt::Vector<uint8_t>(std::make_shared<vkt::VmaBufferAllocation>(this->driver->getAllocator(), vkh::VkBufferCreateInfo{
                .size = requirements.memoryRequirements.size,
                .usage = usage,
            }, vkt::VmaMemoryInfo{ .memUsage = VMA_MEMORY_USAGE_GPU_ONLY }));

            // 
            vkh::handleVk(device->BindAccelerationStructureMemoryKHR(1u, vkh::VkBindAccelerationStructureMemoryInfoKHR{
                .accelerationStructure = this->structure,
                .memory = TempBuffer->getAllocationInfo().memory,
                .memoryOffset = TempBuffer->getAllocationInfo().offset,
            }));
        };

        // 
        if (!this->gpuScratchBuffer.has()) { // 
            vkh::VkMemoryRequirements2 buildReq = {}, updateReq = {};
            device->GetAccelerationStructureMemoryRequirementsKHR(vkh::VkAccelerationStructureMemoryRequirementsInfoKHR{
                .type = VK_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_TYPE_BUILD_SCRATCH_KHR,
                .buildType = VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
                .accelerationStructure = this->structure
            }, buildReq);
            device->GetAccelerationStructureMemoryRequirementsKHR(vkh::VkAccelerationStructureMemoryRequirementsInfoKHR{
                .type = VK_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_TYPE_UPDATE_SCRATCH_KHR,
                .buildType = VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
                .accelerationStructure = this->structure
            }, updateReq);

            //
            auto usage = vkh::VkBufferUsageFlags{.eStorageBuffer = 1, .eRayTracing = 1, .eSharedDeviceAddress = 1 };
            this->gpuScratchBuffer = vkt::Vector<uint8_t>(std::make_shared<vkt::VmaBufferAllocation>(driver->getAllocator(), vkh::VkBufferCreateInfo{
                .size = std::max(buildReq.memoryRequirements.size, updateReq.memoryRequirements.size),
                .usage = usage
            }, vkt::VmaMemoryInfo{ .memUsage = VMA_MEMORY_USAGE_GPU_ONLY }));
        };

        //
        this->updateAccelerationStructure({}, false);
    };

    // buildAccelerationStructureCmd
    void Acceleration::setCommand(const VkCommandBuffer& cmd) {
        this->updateAccelerationStructure({}, false);
        this->driver->getDeviceDispatch()->CmdBuildAccelerationStructureKHR(cmd, 1u, this->bdHeadInfo, reinterpret_cast<VkAccelerationStructureBuildOffsetInfoKHR**>(this->offsetPtr.data()));
    };

    // 
    uint64_t Acceleration::getHandle() {
        return this->driver->getDeviceDispatch()->GetAccelerationStructureDeviceAddressKHR(vkh::VkAccelerationStructureDeviceAddressInfoKHR{ .accelerationStructure = this->structure });
    };


};
