#pragma once
#include "./Config.hpp"

namespace vlr {

    class RayTracing : public std::enable_shared_from_this<RayTracing> { protected: 
        vkt::uni_ptr<Acceleration> accelerationTop = {}; // Top Level Acceleration Structure
        vkt::uni_ptr<VertexSet> vertexSet = {}; // Vertex Set 
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<Driver> driver = {};
        std::vector<vkt::uni_ptr<GeometrySet>> geometries = {};
        std::vector<vkt::uni_ptr<Acceleration>> accelerations = {};
        VkPipeline generation = VK_NULL_HANDLE, interpolation = VK_NULL_HANDLE, intersection = VK_NULL_HANDLE;

    public: 
        RayTracing() { this->constructor(); };
        RayTracing(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
