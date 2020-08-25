#pragma once
#include "./Config.hpp"

namespace vlr {

    using Driver = vkt::GPUFramework;

};

#include "./SetBase.hpp"

namespace vlj {
    class Driver : public Wrap<vlr::Driver> {
        //Driver() : Wrap<vlr::Driver>() {};
        Driver() : Wrap<vlr::Driver>(std::make_shared<vlr::Driver>()) {};

        //CALLIFY(constructor);
    };
};
