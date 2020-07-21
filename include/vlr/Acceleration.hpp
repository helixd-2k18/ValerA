#pragma once
#include "./Config.hpp"
#include "./GeometrySet.hpp"
#include "./InstanceSet.hpp"
#include "./Geometry.hpp"

namespace vlr {

    class Acceleration : public std::enable_shared_from_this<Acceleration> { protected: 
        vkt::uni_ptr<InstanceSet> instanceSet = {};
        vkt::uni_ptr<GeometrySet> geometrySet = {};
        vkt::uni_ptr<Driver> driver = {};

        // FOR CREATE (Acceleration Structure)
        vkh::VkAccelerationStructureCreateInfoKHR                          bottomCreate = {};
        std::vector<vkh::VkAccelerationStructureCreateGeometryTypeInfoKHR> bottomDataCreate = { {} };

        // FOR BUILD! BUT ONLY SINGLE! (Contain Multiple-Instanced)
        vkh::VkAccelerationStructureBuildGeometryInfoKHR              bdHeadInfo = {};
        vkh::VkAccelerationStructureBuildOffsetInfoKHR                offsetTemp = {};
        std::vector<vkh::VkAccelerationStructureBuildOffsetInfoKHR>   offsetInfo = {};
        std::vector<vkh::VkAccelerationStructureBuildOffsetInfoKHR*>  offsetPtr  = {};

        // But used only one, due transform feedback shaders used... 
        std::vector<vkh::VkAccelerationStructureGeometryKHR>   buildGInfo = {};
        std::vector<vkh::VkAccelerationStructureGeometryKHR*>  buildGPtr  = {};
        vkh::VkAccelerationStructureGeometryKHR                buildGTemp = {}; // INSTANCE TEMPLATE, CAN'T BE ARRAY!


    public: 
        Acceleration() { this->constructor(); };
        Acceleration(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, AccelerationCreateInfo info = {}) {
            this->driver = driver;
            offsetInfo.resize(0u);
            buildGInfo.resize(0u);
            offsetPtr.resize(0u);
            buildGPtr.resize(0u);

            // FOR BUILD!
            auto bdHeadFlags = vkh::VkBuildAccelerationStructureFlagsKHR{ .eAllowUpdate = 1, .ePreferFastTrace = 1 };
            this->bdHeadInfo = vkh::VkAccelerationStructureBuildGeometryInfoKHR{};
            this->bdHeadInfo.geometryCount = buildGPtr.size();
            this->bdHeadInfo.ppGeometries = buildGPtr.data();
            this->bdHeadInfo.type = instanceSet.has() ? VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR : VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
            this->bdHeadInfo.flags = bdHeadFlags;
            this->bdHeadInfo.geometryArrayOfPointers = true; // MARK TRUE FOR INDIRECT BUILDING!!

            if (instanceSet.has()) {
                
            } else 
            if (geometrySet.has()) {
                                // FOR CREATE!
                this->bottomDataCreate.resize(info.initials.size()); uintptr_t I = 0ull;
                for (auto& BC : this->bottomDataCreate) {
                    BC.geometryType = this->buildGTemp.geometryType;
                    BC.maxVertexCount = static_cast<uint32_t>(info.initials[I] * 3u);
                    BC.maxPrimitiveCount = static_cast<uint32_t>(info.initials[I]);
                    BC.indexType = VK_INDEX_TYPE_NONE_KHR;
                    BC.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
                    BC.allowsTransforms = true; I++;
                };

                // Generate Building Info
                for (uint32_t i=0u;i<info.geometrySet->geometries.size();i++) {
                    auto geometry = info.geometrySet->geometries[i];
                    auto attribute = geometry->vertexSet->getAttribute(geometry->vertexAttribute);
                    auto binding = geometry->vertexSet->getBinding(attribute.binding);
                    auto offsetDesc = vkh::VkAccelerationStructureBuildOffsetInfoKHR{};
                    auto buildGeometryFlags = vkh::VkGeometryFlagsKHR{ .eNoDuplicateAnyHitInvocation = 1 };
                    auto geometryDesc = vkh::VkAccelerationStructureGeometryKHR{ .flags = buildGeometryFlags };
                    auto triangleDesc = vkh::VkAccelerationStructureGeometryTrianglesDataKHR{};

                    // 
                    triangleDesc.transformData = info.geometrySet->getGpuBuffer();
                    triangleDesc.vertexFormat = attribute.format;
                    triangleDesc.vertexStride = binding.stride;
                    triangleDesc.vertexData = geometry->vertexSet->getAttributeBuffer(geometry->vertexAttribute);
                    triangleDesc.indexType = geometry->indexType;
                    if (geometry->indexBufferView != ~0u && geometry->indexBufferView != -1) {
                        triangleDesc.indexData = geometry->vertexSet->getBuffer(geometry->indexBufferView);
                    };
                    geometryDesc.geometry.triangles = triangleDesc;

                    // 
                    offsetDesc.firstVertex = info.geometrySet->get(i).firstVertex;
                    offsetDesc.primitiveCount = info.geometrySet->get(i).primitiveCount;
                    offsetDesc.transformOffset = i * sizeof(GeometryDesc);
                    offsetDesc.primitiveOffset = 0u;

                    // 
                    buildGInfo.push_back(geometryDesc);
                    offsetInfo.push_back(offsetDesc);
                };

                // 
                for (uint32_t i=0u;i<info.geometrySet->geometries.size();i++) {
                    buildGPtr.push_back(&buildGInfo[i]);
                    offsetPtr.push_back(&offsetInfo[i]);
                };
            };

            // FOR CREATE! 
            this->bottomCreate.maxGeometryCount = this->bottomDataCreate.size();
            this->bottomCreate.pGeometryInfos = this->bottomDataCreate.data();
            this->bottomCreate.type = this->bdHeadInfo.type;
            this->bottomCreate.flags = this->bdHeadInfo.flags;
        };
    };

};
