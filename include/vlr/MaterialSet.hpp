#pragma once
#include "./Config.hpp"

namespace vlr {

    class MaterialSet : public std::enable_shared_from_this<MaterialSet> { protected: 
    public: 
        MaterialSet() { this->constructor(); };

        void constructor() {
            
        };
    };

};
