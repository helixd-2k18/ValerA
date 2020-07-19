#pragma once
#include "./Config.hpp"

namespace vlr {

    class Geometry : public std::enable_shared_from_this<Geometry> { protected: 
    public: 
        Geometry() { this->constructor(); };
        Geometry(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
