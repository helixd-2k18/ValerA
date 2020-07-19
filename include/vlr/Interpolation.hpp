#pragma once
#include "./Config.hpp"

namespace vlr {

    class Interpolation : public std::enable_shared_from_this<Interpolation> { protected: 
    public: 
        Interpolation() { this->constructor(); };
        Interpolation(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        void constructor() {};
        void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
