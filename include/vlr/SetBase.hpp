#pragma once
#include "./Config.hpp"

namespace vlr {

    class SetBase : public std::enable_shared_from_this<SetBase> { protected: 
        

    public: 
        SetBase() { this->constructor(); };
        SetBase(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        void constructor() {};
        void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
