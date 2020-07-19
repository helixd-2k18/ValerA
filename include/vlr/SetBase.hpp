#pragma once
#include "./Config.hpp"

namespace vlr {

    class SetBase : public std::enable_shared_from_this<SetBase> { protected: 
    public: 
        SetBase() { this->constructor(); };

        void constructor() {
            
        };
    };

};
