#pragma once
#include "./Config.hpp"
#include "./VertexSet.hpp"

namespace vlr {

    
    struct GeometryDesc {
        glm::mat3x4 transform = glm::mat3x4(1.f);
        uint32_t firstVertex = 0u;
        uint32_t primitiveCount = 0u;
        uint32_t material = 0u;
        uint32_t flags = 0u;
    };

    class Geometry : public std::enable_shared_from_this<Geometry> { protected: friend GeometrySet; friend Acceleration; friend Rasterization;
        uint32_t vertexAttribute = 0u, indexBufferView = ~0u;
        VkIndexType indexType = VK_INDEX_TYPE_NONE_KHR;
        vkt::uni_ptr<VertexSet> vertexSet = {};
        vkt::uni_arg<GeometryDesc> desc = {};

    public: 
        Geometry() { this->constructor(); };
        Geometry(vkt::uni_ptr<Driver> driver, vkt::uni_ptr<GeometryDesc> desc = {}) { this->constructor(driver, desc); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_ptr<GeometryDesc> desc = {}) {
            this->desc = desc;
        };

        virtual void setIndexBuffer(uint32_t indexBufferView = ~0u, VkIndexType indexType = VK_INDEX_TYPE_NONE_KHR) {
            this->indexBufferView = indexBufferView, this->indexType = indexType;
        };
        virtual void setVertexBuffer(uint32_t vertexAttribute = 0u){
            this->vertexAttribute = vertexAttribute;
        };
    };

};
