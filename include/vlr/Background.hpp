#pragma once
#include "./Config.hpp"

namespace vlr {

    class Background : public std::enable_shared_from_this<Background> { protected: 
    public: 
        Background() { this->constructor(); };

        void constructor() {
            
        };
    };

};
