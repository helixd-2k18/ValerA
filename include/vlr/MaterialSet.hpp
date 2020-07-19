#pragma once
#include "./Config.hpp"

namespace vlr {

    class MaterialSet : public std::enable_shared_from_this<MaterialSet> { protected: 
    public: 
        MaterialSet() { this->constructor(); };
        MaterialSet(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        void constructor() {};
        void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
