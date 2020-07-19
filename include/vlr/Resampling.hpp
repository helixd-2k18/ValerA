#pragma once
#include "./Config.hpp"

namespace vlr {

    class Resampling : public std::enable_shared_from_this<Resampling> { protected: 
    public: 
        Resampling() { this->constructor(); };

        void constructor() {
            
        };
    };

};
