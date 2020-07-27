#pragma once
#include "./Config.hpp"
#include "./vlr/GeometrySet.hpp"
#include "./vlr/InstanceSet.hpp"

namespace vlr {


    struct AccelerationCreateInfo {
        vkt::uni_ptr<InstanceSet> instanceSet = {}; // Top Level
        vkt::uni_ptr<GeometrySet> geometrySet = {}; // Bottom Level
        std::vector<VkDeviceSize> initials = {};
    };

    class Acceleration : public std::enable_shared_from_this<Acceleration> { protected: friend RayTracing; friend PipelineLayout; friend BuildCommand;
        vkt::uni_ptr<InstanceSet> instanceSet = {}; // Top Level
        vkt::uni_ptr<GeometrySet> geometrySet = {}; // Bottom Level
        vkt::uni_ptr<Driver> driver = {};

        // FOR CREATE (Acceleration Structure)
        vkt::uni_arg<AccelerationCreateInfo>                               info = {};
        vkh::VkAccelerationStructureCreateInfoKHR                          create = {};
        std::vector<vkh::VkAccelerationStructureCreateGeometryTypeInfoKHR> dataCreate = {};

        // FOR BUILD! BUT ONLY SINGLE! (Contain Multiple-Instanced)
        vkh::VkAccelerationStructureBuildGeometryInfoKHR              bdHeadInfo = {};
        vkh::VkAccelerationStructureBuildOffsetInfoKHR                offsetTemp = {};
        std::vector<vkh::VkAccelerationStructureBuildOffsetInfoKHR>   offsetInfo = {};
        std::vector<vkh::VkAccelerationStructureBuildOffsetInfoKHR*>  offsetPtr  = {};

        // But used only one, due transform feedback shaders used... 
        std::vector<vkh::VkAccelerationStructureGeometryKHR>   buildGInfo = {};
        std::vector<vkh::VkAccelerationStructureGeometryKHR*>  buildGPtr  = {};
        vkh::VkAccelerationStructureGeometryKHR                buildGTemp = {}; // INSTANCE TEMPLATE, CAN'T BE ARRAY!

        //
        VkDescriptorSet set = VK_NULL_HANDLE; bool updated = false;
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};
        VkAccelerationStructureKHR structure = VK_NULL_HANDLE;
        vkt::Vector<uint8_t> gpuScratchBuffer = {};
        vkt::Vector<uint8_t> TempBuffer = {};

    public: 
        Acceleration() { this->constructor(); };
        Acceleration(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<AccelerationCreateInfo> info = {});
        virtual void updateAccelerationStructure(vkt::uni_arg<AccelerationCreateInfo> info, const bool& build = false);
        virtual void setCommand(const VkCommandBuffer& cmd = VK_NULL_HANDLE); // buildAccelerationStructureCmd
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        virtual uint64_t getHandle();
    };

};
