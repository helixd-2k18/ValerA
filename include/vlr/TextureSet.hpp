#pragma once
#include "./Config.hpp"

namespace vlr {
    
    class TextureSet : public std::enable_shared_from_this<TextureSet> { protected: 
    public: 
        TextureSet() { this->constructor(); };
        TextureSet(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        void constructor() {};
        void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
