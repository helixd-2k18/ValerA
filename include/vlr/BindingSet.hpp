#pragma once
#include "./Config.hpp"

namespace vlr {

    class BindingSet : public std::enable_shared_from_this<BindingSet> { protected: 
    public: 
        BindingSet() { this->constructor(); };

        void constructor() {
            
        };
    };

};
