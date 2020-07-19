#pragma once
#include "./Config.hpp"

namespace vlr {

    class SetBase : public std::enable_shared_from_this<SetBase> { 
    protected: using T = uint8_t;
        VkDescriptorSet set = {}; bool updated = false;
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};
        std::shared_ptr<Driver> driver = {};
        
    private: 
        vkt::VectorBase cpuBuffer = {}, gpuBuffer = {};
        
    public:
        SetBase() { this->constructor(); };
        SetBase(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual vkt::VectorBase getCpuBuffer() const { return cpuBuffer; };
        virtual vkt::VectorBase getGpuBuffer() const { return gpuBuffer; };
        virtual vkt::VectorBase& getCpuBuffer() { return cpuBuffer; };
        virtual vkt::VectorBase& getGpuBuffer() { return gpuBuffer; };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, const uint32_t& count = 1u) {
            //
            auto hostUsage = vkh::VkBufferUsageFlags{.eTransferSrc = 1, .eTransferDst = 1, .eUniformBuffer = 1, .eStorageBuffer = 1, .eRayTracing = 1 };
            auto gpuUsage = vkh::VkBufferUsageFlags{.eTransferSrc = 1, .eTransferDst = 1, .eUniformBuffer = 1, .eStorageBuffer = 1, .eRayTracing = 1 };
            auto upstreamUsage = vkh::VkBufferUsageFlags{ .eTransferSrc = 1, .eTransferDst = 1, .eUniformBuffer = 1, .eStorageBuffer = 1, .eIndirectBuffer = 1, .eRayTracing = 1, .eTransformFeedbackBuffer = 1, .eTransformFeedbackCounterBuffer = 1, .eSharedDeviceAddress = 1 };

            // 
            this->getCpuBuffer() = vkt::VectorBase(std::make_shared<vkt::VmaBufferAllocation>(this->driver->getAllocator(), vkh::VkBufferCreateInfo{ .size = sizeof(T) * count, .usage = hostUsage }, vkt::VmaMemoryInfo{ .memUsage = VMA_MEMORY_USAGE_CPU_TO_GPU }));
            this->getGpuBuffer() = vkt::VectorBase(std::make_shared<vkt::VmaBufferAllocation>(this->driver->getAllocator(), vkh::VkBufferCreateInfo{ .size = sizeof(T) * count, .usage =  gpuUsage }, vkt::VmaMemoryInfo{ .memUsage = VMA_MEMORY_USAGE_GPU_ONLY }));
        };

        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier = false);
    };

    template<class T>
    class SetBase_T : public SetBase { protected:
        
    private: 
        vkt::Vector<T> cpuBuffer = {}, gpuBuffer = {};

    public: 
        virtual vkt::VectorBase getCpuBuffer() const override { return cpuBuffer; };
        virtual vkt::VectorBase getGpuBuffer() const override { return gpuBuffer; };
        virtual vkt::VectorBase& getCpuBuffer() override { return reinterpret_cast<vkt::VectorBase&>(cpuBuffer); };
        virtual vkt::VectorBase& getGpuBuffer() override { return reinterpret_cast<vkt::VectorBase&>(gpuBuffer); };

    };

};
