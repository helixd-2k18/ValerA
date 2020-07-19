#pragma once
#include "./Config.hpp"

namespace vlr {

    class Background : public std::enable_shared_from_this<Background> { protected: 
    public: 
        Background() { this->constructor(); };
        Background(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        void constructor() {};
        void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
