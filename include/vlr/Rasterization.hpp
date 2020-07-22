#pragma once
#include "./Config.hpp"

namespace vlr {

    class Rasterization : public std::enable_shared_from_this<Rasterization> { protected: friend RayTracing;
        vkt::uni_ptr<VertexSet> vertexSet = {};
        vkt::uni_ptr<Geometry> geometry = {};
        vkt::uni_ptr<Interpolation> interpolation = {};
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<Driver> driver = {};
        VkPipeline pipeline = VK_NULL_HANDLE;

    public: 
        Rasterization() { this->constructor(); };
        Rasterization(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
