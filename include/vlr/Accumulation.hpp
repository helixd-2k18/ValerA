#pragma once
#include "./Config.hpp"

namespace vlr {

    class Accumulation : public std::enable_shared_from_this<Accumulation> { protected: 
    public: 
        Accumulation() { this->constructor(); };
        Accumulation(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
