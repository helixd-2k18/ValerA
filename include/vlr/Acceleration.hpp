#pragma once
#include "./Config.hpp"

namespace vlr {

    class Acceleration : public std::enable_shared_from_this<Acceleration> { protected: 
    public: 
        Acceleration() { this->constructor(); };

        void constructor() {
            
        };
    };

};
