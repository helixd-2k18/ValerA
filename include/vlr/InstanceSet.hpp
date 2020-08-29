#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./SetBase.hpp"
#include "./GeometrySet.hpp"
#include "./Acceleration.hpp"

namespace vlr {

    class InstanceSet : public SetBase_T<vkh::VsGeometryInstance> {
    protected: friend Rasterization; friend RayTracing; friend PipelineLayout; friend BuildCommand;
        vkt::uni_ptr<Driver> driver = {};
        std::vector<vkt::uni_ptr<Acceleration>> accelerations = {};

    public: 
        InstanceSet() : SetBase_T<vkh::VsGeometryInstance>() { this->constructorExtend0(); };
        InstanceSet(vkt::uni_ptr<Driver> driver, vkt::uni_arg<DataSetCreateInfo> info = {}) : SetBase_T<vkh::VsGeometryInstance>(driver, info) { this->constructorExtend0(driver, info); };
        ~InstanceSet() {};

        virtual void constructorExtend0() {};
        virtual void constructorExtend0(vkt::uni_ptr<Driver> driver, vkt::uni_arg<DataSetCreateInfo> info = {}) {
            this->driver = driver;
        };

        // 
        virtual const vkt::VectorBase& getCpuBuffer() const override { return SetBase_T<vkh::VsGeometryInstance>::getCpuBuffer(); };
        virtual const vkt::VectorBase& getGpuBuffer() const override { return SetBase_T<vkh::VsGeometryInstance>::getGpuBuffer(); };
        virtual vkt::VectorBase& getCpuBuffer() override { return SetBase_T<vkh::VsGeometryInstance>::getCpuBuffer(); };
        virtual vkt::VectorBase& getGpuBuffer() override { return SetBase_T<vkh::VsGeometryInstance>::getGpuBuffer(); };
        
        // 
        virtual vkh::VsGeometryInstance& get(const uint32_t& I = 0u) override { return SetBase_T<vkh::VsGeometryInstance>::get(I); };
        virtual const vkh::VsGeometryInstance& get(const uint32_t& I = 0u) const override { return SetBase_T<vkh::VsGeometryInstance>::get(I); };

        // 
        vkh::VsGeometryInstance& operator[](const uint32_t& I) { return this->get(I); };
        const vkh::VsGeometryInstance& operator[](const uint32_t& I) const { return this->get(I); };

        // 
        virtual void setAccelerations(std::vector<vkt::uni_ptr<Acceleration>> accelerations);
        virtual void pushAcceleration(vkt::uni_ptr<Acceleration> acceleration);
        virtual void resetAccelerations();
    };

};

namespace vlj {
    class InstanceSet : public Wrap<vlr::InstanceSet> {
    public:
        InstanceSet() : Wrap<vlr::InstanceSet>() {};
        InstanceSet(vkt::uni_ptr<vlr::InstanceSet> object) : Wrap<vlr::InstanceSet>(object) {};
        InstanceSet(vkt::uni_ptr<vlr::Driver> driver, vkt::uni_arg<vlr::DataSetCreateInfo> info = {}) : Wrap<vlr::InstanceSet>(std::make_shared<vlr::InstanceSet>(driver, info)) {};
        InstanceSet(std::shared_ptr<vlr::InstanceSet> object) : Wrap<vlr::InstanceSet>(object) {};

        //CALLIFY(constructor);
        CALLIFY(getCpuBuffer);
        CALLIFY(getGpuBuffer);
        CALLIFY(get);
        CALLIFY(createDescriptorSet);
        CALLIFY(setCommand);
        CALLIFY(getVector);
        CALLIFY(setAccelerations);
        CALLIFY(pushAcceleration);
        CALLIFY(resetAccelerations);
    };
};
