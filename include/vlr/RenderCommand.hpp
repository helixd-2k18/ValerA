#pragma once
#include "./Config.hpp"

namespace vlr {

    class RenderCommand : public std::enable_shared_from_this<RenderCommand> { protected: 
    public: 
        RenderCommand() { this->constructor(); };

        void constructor() {
            
        };
    };

};
