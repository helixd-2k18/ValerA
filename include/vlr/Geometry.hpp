#pragma once
#include "./Config.hpp"

namespace vlr {

    class Geometry : public std::enable_shared_from_this<Geometry> { protected: 
    public: 
        Geometry() { this->constructor(); };

        void constructor() {
            
        };
    };

};
