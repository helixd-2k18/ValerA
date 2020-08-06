#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

#ifdef ENABLE_OPTIX_DENOISE
    class OptiXDenoise : public std::enable_shared_from_this<OptiXDenoise> { protected: 
    public: 
        OptiXDenoise() { this->constructor(); };
        OptiXDenoise(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };
#endif

};
