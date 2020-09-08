#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./GeometrySet.hpp"
#include "./InstanceSet.hpp"

namespace vlr {

    struct AccelerationCreateInfo {
        vkt::uni_ptr<InstanceSet> instanceSet = {}; // Top Level
        vkt::uni_ptr<GeometrySet> geometrySet = {}; // Bottom Level
        std::vector<int64_t> initials = {};
        VkDeviceSize maxInstanceCount = 256ull;

        // Used for monopoly-based Turing architecture
        uintptr_t initialID = 0ull;
    };

    class Acceleration : public std::enable_shared_from_this<Acceleration> { protected: friend RayTracing; friend PipelineLayout; friend BuildCommand; friend Rasterization;
        vkt::uni_ptr<InstanceSet> instanceSet = {}; // Top Level
        vkt::uni_ptr<GeometrySet> geometrySet = {}; // Bottom Level
        vkt::uni_ptr<Driver> driver = {};

        // FOR CREATE (Acceleration Structure)
        //vkt::uni_arg<AccelerationCreateInfo>                               info = {};
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
        vkt::VectorBase gpuScratchBuffer = {};
        vkt::VectorBase TempBuffer = {};

        //
        VkDeviceSize instanceCount = 256ull;
        uintptr_t initialID = 0ull;

    public: 
        Acceleration() { this->constructor(); };
        Acceleration(vkt::uni_ptr<Driver> driver, vkt::uni_arg<AccelerationCreateInfo> info = AccelerationCreateInfo{}) { this->constructor(driver, info); };
        ~Acceleration() {};

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<AccelerationCreateInfo> info = AccelerationCreateInfo{});
        virtual void updateAccelerationStructure(vkt::uni_arg<AccelerationCreateInfo> info, const bool& build = false);
        virtual void setCommand(const VkCommandBuffer& cmd = VK_NULL_HANDLE); // buildAccelerationStructureCmd
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        virtual void setInstanceCount(const VkDeviceSize& count) { this->instanceCount = count; };
        virtual uint64_t getHandle();
    };

};

namespace vlj {
    class Acceleration : public Wrap<vlr::Acceleration> { 
    public:
        Acceleration() : Wrap<vlr::Acceleration>() {};
        Acceleration(vkt::uni_ptr<vlr::Acceleration> object) : Wrap<vlr::Acceleration>(object) {};
        Acceleration(vkt::uni_ptr<vlr::Driver> driver, vkt::uni_arg<vlr::AccelerationCreateInfo> info = vlr::AccelerationCreateInfo{}) : Wrap<vlr::Acceleration>(std::make_shared<vlr::Acceleration>(driver, info)) {};
        Acceleration(std::shared_ptr<vlr::Acceleration> object) : Wrap<vlr::Acceleration>(object) {};

        //CALLIFY(constructor);
        CALLIFY(updateAccelerationStructure);
        CALLIFY(setCommand);
        CALLIFY(createDescriptorSet);
        CALLIFY(getHandle);
        CALLIFY(setInstanceCount);
    };
};
