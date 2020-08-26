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

namespace vlj {
    class OthersCommand : public Wrap<vlr::OthersCommand> {
        OthersCommand() : Wrap<vlr::OthersCommand>() {};
        OthersCommand(vkt::uni_ptr<vlr::OthersCommand> object) : Wrap<vlr::OthersCommand>(object) {};
        OthersCommand(vkt::uni_ptr<vlr::Driver> driver) : Wrap<vlr::OthersCommand>(std::make_shared<vlr::OthersCommand>(driver)) {};

        //CALLIFY(constructor);
    };
};