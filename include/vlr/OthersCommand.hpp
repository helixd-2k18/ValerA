#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

    class OthersCommand : public std::enable_shared_from_this<OthersCommand> { protected: 
    public: 
        OthersCommand() { this->constructor(); };
        OthersCommand(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
