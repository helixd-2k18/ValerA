#pragma once
#include "./Config.hpp"

namespace vlr {

    class RenderCommand : public std::enable_shared_from_this<RenderCommand> { protected: 
        vkt::uni_ptr<RayTracing> raytracing = {};
        vkt::uni_ptr<Rasterization> rasterization = {};

    public: 
        RenderCommand() { this->constructor(); };
        RenderCommand(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
