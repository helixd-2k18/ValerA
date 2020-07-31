#include "./vlr/Acceleration.hpp"
#include "./vlr/PipelineLayout.hpp"
#include "./vlr/GeometrySet.hpp"
#include "./vlr/InstanceSet.hpp"
#include "./vlr/Geometry.hpp"

namespace vlr {

    // 
    void Acceleration::createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) {
        this->descriptorSetInfo = vkh::VsDescriptorSetCreateInfoHelper(pipelineLayout->getAccelerationSetLayout(), pipelineLayout->getDescriptorPool());
        auto& handle = this->descriptorSetInfo.pushDescription(vkh::VkDescriptorUpdateTemplateEntry{
            .dstBinding = 0u,
            .dstArrayElement = 0u,
            .descriptorCount = 1u,
            .descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR
        });
        handle.offset<VkAccelerationStructureKHR>(0) = this->structure;
        vkh::handleVk(vkt::AllocateDescriptorSetWithUpdate(driver->getDeviceDispatch(), this->descriptorSetInfo, this->set, this->updated));
    };

    // 
    void Acceleration::updateAccelerationStructure(vkt::uni_arg<AccelerationCreateInfo> info, const bool& build) {
        offsetInfo.resize(0u);
        buildGInfo.resize(0u);
        offsetPtr.resize(0u);
        buildGPtr.resize(0u);

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

                // 
                //std::cout << "Instance Sizeof = " << sizeof(vkh::VsGeometryInstance) << std::endl;
                instanceDesc.data = instanceSet->getGpuBuffer()->deviceAddress();
                instanceDesc.arrayOfPointers = false;
                geometryDesc.geometry.instances = instanceDesc;
                geometryDesc.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;

                // 
                offsetDesc.primitiveCount = uint32_t(instanceSet->getGpuBuffer().size());
                offsetDesc.transformOffset = 0u;
                offsetDesc.primitiveOffset = instanceSet->getGpuBuffer().offset();
                offsetDesc.firstVertex = 0u;

                // 
                buildGInfo.push_back(geometryDesc);
                offsetInfo.push_back(offsetDesc);
            };
        } else 
        if (geometrySet.has()) {
            // Generate Building Info
            for (uint32_t i=0u;i<geometrySet->geometries.size();i++) 
            {   // 
                auto geometry = geometrySet->geometries[i];
                auto attribute = geometry->vertexSet->getAttribute(geometry->desc->vertexAttribute);
                auto binding = geometry->vertexSet->getBinding(attribute.binding);
                auto offsetDesc = vkh::VkAccelerationStructureBuildOffsetInfoKHR{};
                auto buildGeometryFlags = vkh::VkGeometryFlagsKHR{ .eNoDuplicateAnyHitInvocation = 1 };
                auto geometryDesc = vkh::VkAccelerationStructureGeometryKHR{ .flags = buildGeometryFlags };
                auto triangleDesc = vkh::VkAccelerationStructureGeometryTrianglesDataKHR{};
                auto buffer = geometry->vertexSet->getAttributeBuffer(geometry->desc->vertexAttribute);

                // 
                triangleDesc.transformData = geometrySet->getGpuBuffer()->deviceAddress();
                triangleDesc.vertexFormat = attribute.format;
                triangleDesc.vertexStride = binding.stride;
                triangleDesc.vertexData = geometry->vertexSet->getAttributeBuffer(geometry->desc->vertexAttribute)->deviceAddress();

                // 
                if (geometry->desc->indexBufferView != ~0u && geometry->desc->indexBufferView != -1 && geometry->desc->indexType != VK_INDEX_TYPE_NONE_KHR) {
                    triangleDesc.indexData = geometry->vertexSet->getBuffer(geometry->desc->indexBufferView);
                    triangleDesc.indexType = geometry->desc->indexType;
                } else {
                    triangleDesc.indexType = VK_INDEX_TYPE_NONE_KHR;
                    triangleDesc.indexData = VK_NULL_HANDLE;
                };

                // Fix vec4 formats into vec3, without alpha (but still can be passed by stride value)
                if (triangleDesc.vertexFormat == VK_FORMAT_R32G32B32A32_SFLOAT) triangleDesc.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
                if (triangleDesc.vertexFormat == VK_FORMAT_R16G16B16A16_SFLOAT) triangleDesc.vertexFormat = VK_FORMAT_R16G16B16_SFLOAT;
                geometryDesc.geometry.triangles = triangleDesc;
                geometryDesc.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;

                // 
                offsetDesc.firstVertex = geometry->desc->firstVertex;
                offsetDesc.primitiveCount = geometry->desc->primitiveCount;
                offsetDesc.transformOffset = i * sizeof(GeometryDesc);
                offsetDesc.primitiveOffset = uint32_t(buffer.offset());

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
        this->bdHeadInfo.geometryArrayOfPointers = false;
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
        this->driver = driver;
        if (info.has()) {
            this->geometrySet = info->geometrySet;
            this->instanceSet = info->instanceSet;
        };
        auto device = this->driver->getDeviceDispatch();

        // FOR CREATE!
        this->dataCreate.resize(info->initials.size()); uintptr_t I = 0ull;
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
            vkh::VkMemoryRequirements2 requirements = {};
            device->GetAccelerationStructureMemoryRequirementsKHR(vkh::VkAccelerationStructureMemoryRequirementsInfoKHR{
                .type = VK_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_TYPE_BUILD_SCRATCH_KHR,
                .buildType = VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
                .accelerationStructure = this->structure
            }, requirements);

            //
            auto usage = vkh::VkBufferUsageFlags{.eStorageBuffer = 1, .eRayTracing = 1, .eSharedDeviceAddress = 1 };
            this->gpuScratchBuffer = vkt::Vector<uint8_t>(std::make_shared<vkt::VmaBufferAllocation>(driver->getAllocator(), vkh::VkBufferCreateInfo{
                .size = requirements.memoryRequirements.size,
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
