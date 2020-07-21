#include "./vlr/Acceleration.hpp"
#include "./vlr/SetBase.hpp"
#include "./vlr/GeometrySet.hpp"
#include "./vlr/InstanceSet.hpp"
#include "./vlr/Geometry.hpp"

namespace vlr {

    void Acceleration::updateAccelerationStructure(vkt::uni_arg<AccelerationCreateInfo> info, const bool& build) {
        offsetInfo.resize(0u);
        buildGInfo.resize(0u);
        offsetPtr.resize(0u);
        buildGPtr.resize(0u);

        // 
        if (instanceSet.has()) {
            
        } else 
        if (geometrySet.has()) {
            // Generate Building Info
            for (uint32_t i=0u;i<info->geometrySet->geometries.size();i++) {
                auto geometry = info->geometrySet->geometries[i];
                auto attribute = geometry->vertexSet->getAttribute(geometry->vertexAttribute);
                auto binding = geometry->vertexSet->getBinding(attribute.binding);
                auto offsetDesc = vkh::VkAccelerationStructureBuildOffsetInfoKHR{};
                auto buildGeometryFlags = vkh::VkGeometryFlagsKHR{ .eNoDuplicateAnyHitInvocation = 1 };
                auto geometryDesc = vkh::VkAccelerationStructureGeometryKHR{ .flags = buildGeometryFlags };
                auto triangleDesc = vkh::VkAccelerationStructureGeometryTrianglesDataKHR{};

                // 
                triangleDesc.transformData = info->geometrySet->getGpuBuffer();
                triangleDesc.vertexFormat = attribute.format;
                triangleDesc.vertexStride = binding.stride;
                triangleDesc.vertexData = geometry->vertexSet->getAttributeBuffer(geometry->vertexAttribute);
                triangleDesc.indexType = geometry->indexType;
                if (geometry->indexBufferView != ~0u && geometry->indexBufferView != -1) {
                    triangleDesc.indexData = geometry->vertexSet->getBuffer(geometry->indexBufferView);
                };
                geometryDesc.geometry.triangles = triangleDesc;

                // 
                offsetDesc.firstVertex = (info->geometrySet)->get(i).firstVertex;
                offsetDesc.primitiveCount = (info->geometrySet)->get(i).primitiveCount;
                offsetDesc.transformOffset = i * sizeof(GeometryDesc);
                offsetDesc.primitiveOffset = 0u;

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
        auto bdHeadFlags = vkh::VkBuildAccelerationStructureFlagsKHR{ .eAllowUpdate = 1, .ePreferFastTrace = 1 };
        this->create.maxGeometryCount = this->dataCreate.size();
        this->create.pGeometryInfos = this->dataCreate.data();
        this->create.type = instanceSet.has() ? VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR : VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
        this->create.flags = bdHeadFlags;

        //
        this->updateAccelerationStructure(info, false);
    };

    void Acceleration::buildAccelerationStructureCmd(const VkCommandBuffer& cmd) {
        this->updateAccelerationStructure(info, false);
        driver->getDeviceDispatch()->CmdBuildAccelerationStructureKHR(cmd, 1u, this->bdHeadInfo, reinterpret_cast<VkAccelerationStructureBuildOffsetInfoKHR**>(this->offsetPtr.data()));
    };

};
