#pragma once
#include "./Config.hpp"

namespace vlr {
    
    class TextureSet : public std::enable_shared_from_this<TextureSet> { protected: 
    public: 
        TextureSet() { this->constructor(); };

        void constructor() {
            
        };
    };

};
