#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

    class SetBase : public std::enable_shared_from_this<SetBase> { friend RayTracing; friend Rasterization; friend PipelineLayout;
    protected: //using T = uint8_t;
        VkDescriptorSet set = VK_NULL_HANDLE; bool updated = false;
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};
        vkt::uni_ptr<Driver> driver = {};
        bool uniform = false, enableCPU = true, enableGL = false;
        
        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<DataSetCreateInfo> info = {}, const uint32_t& stride = 1u);

    private: 
        vkt::VectorBase cpuBuffer = {}, gpuBuffer = {};
        
    public:
        SetBase() { this->constructor(); };
        SetBase(vkt::uni_ptr<Driver> driver, vkt::uni_arg<DataSetCreateInfo> info = {}, const uint32_t& stride = 1u) { this->constructor(driver, info, stride); };
        ~SetBase() {};

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
        SetBase_T(vkt::uni_ptr<Driver> driver, vkt::uni_arg<DataSetCreateInfo> info = {}) { this->constructor(driver, info, sizeof(T)); };
        ~SetBase_T() {};

        // 
        virtual const vkt::VectorBase& getCpuBuffer() const override { return reinterpret_cast<const vkt::VectorBase&>(cpuBuffer); };
        virtual const vkt::VectorBase& getGpuBuffer() const override { return reinterpret_cast<const vkt::VectorBase&>(gpuBuffer); };
        virtual vkt::VectorBase& getCpuBuffer() override { return reinterpret_cast<vkt::VectorBase&>(cpuBuffer); };
        virtual vkt::VectorBase& getGpuBuffer() override { return reinterpret_cast<vkt::VectorBase&>(gpuBuffer); };

        // 
        virtual T& get(const uint32_t& I = 0u) { return dynamic_cast<vkt::Vector<T>&>(this->getCpuBuffer()).at(I); };
        virtual const T& get(const uint32_t& I = 0u) const { return dynamic_cast<const vkt::Vector<T>&>(this->getCpuBuffer()).at(I); };

        // 
        T& operator[](const uint32_t& I) { return this->get(I); };
        const T& operator[](const uint32_t& I) const { return this->get(I); };

        // 
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) override { SetBase::createDescriptorSet(pipelineLayout); };
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier = false) override { SetBase::setCommand(commandBuffer, barrier); };

        // for DEBUG
        virtual std::vector<T> getVector() { std::vector<T> acp(this->getCpuBuffer().size()); memcpy(acp.data(), cpuBuffer.data(), acp.size() * sizeof(T)); return acp; };
    };

};

namespace vlj {
    class SetBase : public Wrap<vlr::SetBase> {
    public:
        SetBase() : Wrap<vlr::SetBase>() {};
        SetBase(vkt::uni_ptr<vlr::SetBase> object) : Wrap<vlr::SetBase>(object) {};
        SetBase(vkt::uni_ptr<vlr::Driver> driver, vkt::uni_arg<vlr::DataSetCreateInfo> info = {}, const uint32_t& stride = 1u) : Wrap<vlr::SetBase>(std::make_shared<vlr::SetBase>(driver, info, stride)) {};
        SetBase(std::shared_ptr<vlr::SetBase> object) : Wrap<vlr::SetBase>(object) {};

        //CALLIFY(constructor);
        CALLIFY(getCpuBuffer);
        CALLIFY(getGpuBuffer);
        //CALLIFY(get);
        CALLIFY(createDescriptorSet);
        CALLIFY(setCommand);
        //CALLIFY(getVector);
    };
};
