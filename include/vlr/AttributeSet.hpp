#pragma once
#include "./Config.hpp"

namespace vlr {

    class AttributeSet : public std::enable_shared_from_this<AttributeSet> { protected: 
    public: 
        AttributeSet() { this->constructor(); };

        void constructor() {
            
        };
    };

};
