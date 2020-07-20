#pragma once
#include "./Config.hpp"

namespace vlr {

    class Rasterization : public std::enable_shared_from_this<Rasterization> { protected: 
        vkt::uni_ptr<Geometry> geometry = {};
        vkt::uni_ptr<Interpolation> interpolation = {};

    public: 
        Rasterization() { this->constructor(); };
        Rasterization(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
