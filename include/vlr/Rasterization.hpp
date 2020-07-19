#pragma once
#include "./Config.hpp"

namespace vlr {

    class Rasterization : public std::enable_shared_from_this<Rasterization> { protected: 
    public: 
        Rasterization() { this->constructor(); };
        Rasterization(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        void constructor() {};
        void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
