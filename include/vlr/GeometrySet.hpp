#pragma once
#include "./Config.hpp"
#include "./SetBase.hpp"

// 
namespace vlr {

    struct GeometryDesc {
        glm::mat3x4 transform = glm::mat3x4(1.f);
        uint32_t firstVertex = 0u;
        uint32_t primitiveCount = 0u;
        uint32_t material = 0u;
        uint32_t flags = 0u;
    };

    class GeometrySet : public SetBase_T<GeometryDesc> { protected: 
        std::vector<Geometry> geometries = {};

    public: 
        GeometrySet() : SetBase_T<GeometryDesc>() { this->constructorExtend0(); };
        GeometrySet(vkt::uni_ptr<Driver> driver, vkt::uni_arg<DataSetCreateInfo> info) : SetBase_T<GeometryDesc>(driver, info) { this->constructorExtend0(driver); };

        virtual void constructorExtend0() {};
        virtual void constructorExtend0(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
