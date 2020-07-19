#pragma once
#include "./Config.hpp"

namespace vlr {

    class Framebuffer : public std::enable_shared_from_this<Framebuffer> { protected: 
    public: 
        Framebuffer() { this->constructor(); };

        void constructor() {
            
        };
    };

};
