#pragma once
#include "./Config.hpp"

namespace vlr {
    
    class BuildCommand : public std::enable_shared_from_this<BuildCommand> { protected: 
    public: 
        BuildCommand() { this->constructor(); };

        void constructor() {
            
        };
    };

};
