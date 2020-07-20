#pragma once
#include "./Config.hpp"

namespace vlr {

    class Acceleration : public std::enable_shared_from_this<Acceleration> { protected: 
        vkt::uni_ptr<GeometrySet> geometry = {};

    public: 
        Acceleration() { this->constructor(); };
        Acceleration(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
