#pragma once
#include "./Config.hpp"

namespace vlr {

    class Driver : public std::enable_shared_from_this<Driver> { protected: 
    public: 
        Driver(vkt::uni_ptr<xvk::Device> device) { this->constructor(device); };
        Driver() { this->constructor(); };

        void constructor() {};
        void constructor(vkt::uni_ptr<xvk::Device> device) {
            
        };
    };

};
