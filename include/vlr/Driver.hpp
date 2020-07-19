#pragma once
#include "./Config.hpp"

namespace vlr {

    class Driver : public std::enable_shared_from_this<Driver> { protected: 
    public: 
        Driver() { this->constructor(); };

        void constructor() {
            
        };
    };

};
