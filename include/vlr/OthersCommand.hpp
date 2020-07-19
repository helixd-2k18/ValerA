#pragma once
#include "./Config.hpp"

namespace vlr {

    class OthersCommand : public std::enable_shared_from_this<OthersCommand> { protected: 
    public: 
        OthersCommand() { this->constructor(); };

        void constructor() {
            
        };
    };

};
