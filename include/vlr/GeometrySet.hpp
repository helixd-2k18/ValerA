#pragma once
#include "./Config.hpp"

// 
// 
// 
namespace vlr {

    struct GeometryDesc {
        glm::mat3x4 transform = glm::mat3x4(1.f);
        uint32_t offset = 0u;
        uint32_t material = 0u;
    };

    class GeometrySet : public std::enable_shared_from_this<GeometrySet> { protected: 
    public: 
        GeometrySet() { this->constructor(); };
        GeometrySet(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
