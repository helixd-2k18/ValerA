#pragma once
#include "./Config.hpp"
#include "./SetBase.hpp"

namespace vlr {

    class InstanceSet : public SetBase_T<vkh::VsGeometryInstance> { protected: 
        vkt::uni_ptr<Driver> driver = {};

    public: 
        InstanceSet() : SetBase_T<vkh::VsGeometryInstance>() { this->constructorExtend0(); };
        InstanceSet(vkt::uni_ptr<Driver> driver, vkt::uni_arg<DataSetCreateInfo> info) : SetBase_T<vkh::VsGeometryInstance>(driver, info) { this->constructorExtend0(driver); };

        virtual void constructorExtend0() {};
        virtual void constructorExtend0(vkt::uni_ptr<Driver> driver) {};
    };

};
