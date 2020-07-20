#pragma once
#include "./Config.hpp"
#include "./VertexSet.hpp"

namespace vlr {

    class Geometry : public std::enable_shared_from_this<Geometry> { protected: friend GeometrySet;
        uint32_t vertexAttribute = 0u, indexBufferView = ~0u;
        VkIndexType indexType = VK_INDEX_TYPE_NONE_KHR;
        vkt::uni_ptr<VertexSet> vertexSet = {};

    public: 
        Geometry() { this->constructor(); };
        Geometry(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
