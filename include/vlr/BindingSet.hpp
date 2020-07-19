#pragma once
#include "./Config.hpp"

namespace vlr {

    class BindingSet : public std::enable_shared_from_this<BindingSet> { protected: 
    public: 
        BindingSet() { this->constructor(); };
        BindingSet(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        void constructor() {};
        void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
