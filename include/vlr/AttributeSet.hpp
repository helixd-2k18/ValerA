#pragma once
#include "./Config.hpp"

namespace vlr {

    class AttributeSet : public std::enable_shared_from_this<AttributeSet> { protected: 
    public: 
        AttributeSet() { this->constructor(); };
        AttributeSet(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        void constructor() {};
        void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
