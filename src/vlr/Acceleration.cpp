#include "./vlr/Acceleration.hpp"
#include "./vlr/PipelineLayout.hpp"
#include "./vlr/SetBase.hpp"
#include "./vlr/GeometrySet.hpp"
#include "./vlr/InstanceSet.hpp"
#include "./vlr/Geometry.hpp"

namespace vlr {

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

    void Acceleration::updateAccelerationStructure(vkt::uni_arg<AccelerationCreateInfo> info, const bool& build) {
        offsetInfo.resize(0u);
        buildGInfo.resize(0u);
        offsetPtr.resize(0u);
        buildGPtr.resize(0u);

        // 
        if (instanceSet.has()) {
            uint32_t i = 0u;
            {   // 
                auto geometry = info->geometrySet->geometries[i];
                auto attribute = geometry->vertexSet->getAttribute(geometry->vertexAttribute);
                auto binding = geometry->vertexSet->getBinding(attribute.binding);
                auto offsetDesc = vkh::VkAccelerationStructureBuildOffsetInfoKHR{};
                auto buildGeometryFlags = vkh::VkGeometryFlagsKHR{ .eNoDuplicateAnyHitInvocation = 1 };
                auto geometryDesc = vkh::VkAccelerationStructureGeometryKHR{ .flags = buildGeometryFlags };
                auto instanceDesc = vkh::VkAccelerationStructureGeometryInstancesDataKHR{};

                // 
                instanceDesc.data = info->instanceSet->getCpuBuffer();
                geometryDesc.geometry.instances = instanceDesc;

                // 
                offsetDesc.primitiveCount = info->instanceSet->getCpuBuffer().size();
                offsetDesc.transformOffset = info->instanceSet->getCpuBuffer().offset();
                offsetDesc.primitiveOffset = 0u;
                offsetDesc.firstVertex = 0u;

                // 
                buildGInfo.push_back(geometryDesc);
                offsetInfo.push_back(offsetDesc);
            };
        } else 
        if (geometrySet.has()) {
            // Generate Building Info
            for (uint32_t i=0u;i<info->geometrySet->geometries.size();i++) 
            {   // 
                auto geometry = info->geometrySet->geometries[i];
                auto attribute = geometry->vertexSet->getAttribute(geometry->vertexAttribute);
                auto binding = geometry->vertexSet->getBinding(attribute.binding);
                auto offsetDesc = vkh::VkAccelerationStructureBuildOffsetInfoKHR{};
                auto buildGeometryFlags = vkh::VkGeometryFlagsKHR{ .eNoDuplicateAnyHitInvocation = 1 };
                auto geometryDesc = vkh::VkAccelerationStructureGeometryKHR{ .flags = buildGeometryFlags };
                auto triangleDesc = vkh::VkAccelerationStructureGeometryTrianglesDataKHR{};
                auto buffer = geometry->vertexSet->getAttributeBuffer(geometry->vertexAttribute);

                // 
                triangleDesc.transformData = info->geometrySet->getGpuBuffer();
                triangleDesc.vertexFormat = attribute.format;
                triangleDesc.vertexStride = binding.stride;
                triangleDesc.vertexData = geometry->vertexSet->getAttributeBuffer(geometry->vertexAttribute);
                
                // 
                if (geometry->indexBufferView != ~0u && geometry->indexBufferView != -1 && geometry->indexType != VK_INDEX_TYPE_NONE_KHR) {
                    triangleDesc.indexData = geometry->vertexSet->getBuffer(geometry->indexBufferView);
                    triangleDesc.indexType = geometry->indexType;
                } else {
                    triangleDesc.indexType = VK_INDEX_TYPE_NONE_KHR;
                    triangleDesc.indexData = VK_NULL_HANDLE;
                };

                // Fix vec4 formats into vec3, without alpha (but still can be passed by stride value)
                if (triangleDesc.vertexFormat == VK_FORMAT_R32G32B32A32_SFLOAT) triangleDesc.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
                if (triangleDesc.vertexFormat == VK_FORMAT_R16G16B16A16_SFLOAT) triangleDesc.vertexFormat = VK_FORMAT_R16G16B16_SFLOAT;
                geometryDesc.geometry.triangles = triangleDesc;

                // 
                offsetDesc.firstVertex = (info->geometrySet)->get(i).firstVertex;
                offsetDesc.primitiveCount = (info->geometrySet)->get(i).primitiveCount;
                offsetDesc.transformOffset = i * sizeof(GeometryDesc);
                offsetDesc.primitiveOffset = buffer.offset();

                // 
                buildGInfo.push_back(geometryDesc);
                offsetInfo.push_back(offsetDesc);
            };
        };

        // 
        for (uint32_t i=0u;i<info->geometrySet->geometries.size();i++) {
            buildGPtr.push_back(&buildGInfo[i]);
            offsetPtr.push_back(&offsetInfo[i]);
        };

        // FOR BUILD!
        auto bdHeadFlags = vkh::VkBuildAccelerationStructureFlagsKHR{ .eAllowUpdate = 1, .ePreferFastTrace = 1 };
        this->bdHeadInfo = vkh::VkAccelerationStructureBuildGeometryInfoKHR{};
        this->bdHeadInfo.geometryCount = buildGPtr.size();
        this->bdHeadInfo.ppGeometries = buildGPtr.data();
        this->bdHeadInfo.type = instanceSet.has() ? VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR : VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
        this->bdHeadInfo.flags = bdHeadFlags;
        this->bdHeadInfo.geometryArrayOfPointers = true;
        this->bdHeadInfo.dstAccelerationStructure = this->structure;

        // Only For Supported GPU's
        if (build) {
            vkh::handleVk(driver->getDeviceDispatch()->BuildAccelerationStructureKHR(1u, this->bdHeadInfo, reinterpret_cast<VkAccelerationStructureBuildOffsetInfoKHR**>(this->offsetPtr.data())));
        };
    };

    void Acceleration::constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<AccelerationCreateInfo> info) {
        this->driver = driver, this->info = info;

        // FOR CREATE!
        this->dataCreate.resize(info->initials.size()); uintptr_t I = 0ull;
        for (auto& BC : this->dataCreate) {
            BC.geometryType = instanceSet.has() ? VK_GEOMETRY_TYPE_INSTANCES_KHR : VK_GEOMETRY_TYPE_TRIANGLES_KHR;
            BC.maxVertexCount = static_cast<uint32_t>(info->initials[I] * 3u);
            BC.maxPrimitiveCount = static_cast<uint32_t>(info->initials[I]);
            BC.indexType = VK_INDEX_TYPE_NONE_KHR;
            BC.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
            BC.allowsTransforms = true; I++;
        };

        // FOR CREATE! 
        {
            auto bdHeadFlags = vkh::VkBuildAccelerationStructureFlagsKHR{ .eAllowUpdate = 1, .ePreferFastTrace = 1 };
            this->create.maxGeometryCount = this->dataCreate.size();
            this->create.pGeometryInfos = this->dataCreate.data();
            this->create.type = instanceSet.has() ? VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR : VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
            this->create.flags = bdHeadFlags;
        };
        
        // 
        if (!this->structure) { // create acceleration structure fastly...
            vkh::handleVk(driver->getDeviceDispatch()->CreateAccelerationStructureKHR(this->create, nullptr, &this->structure));

            //
            vkh::VkMemoryRequirements2 requirements = {};
            driver->getDeviceDispatch()->GetAccelerationStructureMemoryRequirementsKHR(vkh::VkAccelerationStructureMemoryRequirementsInfoKHR{
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
            vkh::handleVk(driver->getDeviceDispatch()->BindAccelerationStructureMemoryKHR(1u, vkh::VkBindAccelerationStructureMemoryInfoKHR{
                .accelerationStructure = this->structure,
                .memory = TempBuffer->getAllocationInfo().memory,
                .memoryOffset = TempBuffer->getAllocationInfo().offset,
            }));
        };

        // 
        if (!this->gpuScratchBuffer.has()) { // 
            vkh::VkMemoryRequirements2 requirements = {};
            driver->getDeviceDispatch()->GetAccelerationStructureMemoryRequirementsKHR(vkh::VkAccelerationStructureMemoryRequirementsInfoKHR{
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
        this->updateAccelerationStructure(info, false);
    };

    void Acceleration::buildAccelerationStructureCmd(const VkCommandBuffer& cmd) {
        this->updateAccelerationStructure(info, false);
        driver->getDeviceDispatch()->CmdBuildAccelerationStructureKHR(cmd, 1u, this->bdHeadInfo, reinterpret_cast<VkAccelerationStructureBuildOffsetInfoKHR**>(this->offsetPtr.data()));
    };


};
