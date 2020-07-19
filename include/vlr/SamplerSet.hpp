#pragma once
#include "./Config.hpp"

namespace vlr {

    class SamplerSet : public std::enable_shared_from_this<SamplerSet> { protected: 
    public: 
        SamplerSet() { this->constructor(); };

        void constructor() {
            
        };
    };

};
