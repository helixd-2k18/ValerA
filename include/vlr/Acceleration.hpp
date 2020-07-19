#pragma once
#include "./Config.hpp"

namespace vlr {

    class Acceleration : public std::enable_shared_from_this<Acceleration> { protected: 
    public: 
        Acceleration() { this->constructor(); };
        Acceleration(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        void constructor() {};
        void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
