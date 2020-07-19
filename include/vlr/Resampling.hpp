#pragma once
#include "./Config.hpp"

namespace vlr {

    class Resampling : public std::enable_shared_from_this<Resampling> { protected: 
    public: 
        Resampling() { this->constructor(); };
        Resampling(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        void constructor() {};
        void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
