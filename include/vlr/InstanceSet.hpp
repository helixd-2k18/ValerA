#pragma once
#include "./Config.hpp"

namespace vlr {

    class InstanceSet : public std::enable_shared_from_this<InstanceSet> { protected: 
    public: 
        InstanceSet() { this->constructor(); };
        InstanceSet(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
