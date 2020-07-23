#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

    class SetBase : public std::enable_shared_from_this<SetBase> { friend RayTracing; friend Rasterization; friend PipelineLayout;
    protected: //using T = uint8_t;
        VkDescriptorSet set = VK_NULL_HANDLE; bool updated = false;
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};
        vkt::uni_ptr<Driver> driver = {};
        bool uniform = false;
        
        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, const uint32_t& stride = 1u, vkt::uni_arg<DataSetCreateInfo> info = {});

    private: 
        vkt::VectorBase cpuBuffer = {}, gpuBuffer = {};
        
    public:
        SetBase() { this->constructor(); };
        SetBase(vkt::uni_ptr<Driver> driver, vkt::uni_arg<DataSetCreateInfo> info = {}) { this->constructor(driver, 1u, info); };

        // 
        virtual const vkt::VectorBase& getCpuBuffer() const { return cpuBuffer; };
        virtual const vkt::VectorBase& getGpuBuffer() const { return gpuBuffer; };
        virtual vkt::VectorBase& getCpuBuffer() { return cpuBuffer; };
        virtual vkt::VectorBase& getGpuBuffer() { return gpuBuffer; };

        // 
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier = false);
    };

    template<class T>
    class SetBase_T : public SetBase { protected: friend Acceleration;
        
    private: 
        vkt::Vector<T> cpuBuffer = {}, gpuBuffer = {};

    public: 
        SetBase_T() : SetBase() { this->constructor(); };
        SetBase_T(vkt::uni_ptr<Driver> driver, vkt::uni_arg<DataSetCreateInfo> info = {}) { this->constructor(driver, sizeof(T), info); };

        // 
        virtual const vkt::VectorBase& getCpuBuffer() const override { return reinterpret_cast<const vkt::VectorBase&>(cpuBuffer); };
        virtual const vkt::VectorBase& getGpuBuffer() const override { return reinterpret_cast<const vkt::VectorBase&>(gpuBuffer); };
        virtual vkt::VectorBase& getCpuBuffer() override { return reinterpret_cast<vkt::VectorBase&>(cpuBuffer); };
        virtual vkt::VectorBase& getGpuBuffer() override { return reinterpret_cast<vkt::VectorBase&>(gpuBuffer); };

        // 
        virtual T& get(const uint32_t& I = 0u) { return this->getCpuBuffer()[I]; };
        virtual const T& get(const uint32_t& I = 0u) const { return this->getCpuBuffer()[I]; };

        // 
        T& operator[](const uint32_t& I) { return this->get(I); };
        const T& operator[](const uint32_t& I) const { return this->get(I); };
    };

};
