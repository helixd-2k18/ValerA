#pragma once
#include "./Config.hpp"

namespace vlr {

    class BufferViewSet : public std::enable_shared_from_this<BufferViewSet> { protected: 
    public: 
        BufferViewSet() { this->constructor(); };

        void constructor() {
            
        };
    };

};
