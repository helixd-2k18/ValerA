#pragma once
#include "./Config.hpp"

namespace vlr {

    class SamplerSet : public std::enable_shared_from_this<SamplerSet> { protected: 
    public: 
        SamplerSet() { this->constructor(); };
        SamplerSet(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        void constructor() {};
        void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
