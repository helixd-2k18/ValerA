#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./VertexSet.hpp"

namespace vlr {

#pragma pack(push, 1)
    struct GeometryDesc {
        glm::mat3x4 transform = glm::mat3x4(1.f);
        uint32_t firstVertex = 0u;
        uint32_t primitiveCount = 0u;
        uint32_t material = 0u;
        
        // 
        vkh::uint24_t meshID = 0u;
        uint8_t flags = 0u;

        // We solved to re-port into... 
        uint32_t vertexAttribute = 0u, indexBufferView = ~0u;
        VkIndexType indexType = VK_INDEX_TYPE_NONE_KHR;
        uint32_t reserved; // ?? 
    };
#pragma pack(pop)

    class Geometry : public std::enable_shared_from_this<Geometry> { protected: friend GeometrySet; friend Acceleration; friend Rasterization;
        //vkt::uni_ptr<VertexSet> vertexSet = {}; // BROKEN!
        vkt::uni_ptr<VertexSet> vertexSet = {};
        vkt::uni_arg<GeometryDesc> desc = {};

    public: 
        Geometry() { this->constructor(); };
        Geometry(vkt::uni_ptr<Driver> driver, vkt::uni_ptr<GeometryDesc> desc = {}) { this->constructor(driver, desc); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_ptr<GeometryDesc> desc = {}) {
            vkt::Vector<uint8_t> buffer = this->vertexSet->getBuffer_T(this->desc->vertexAttribute);
            this->desc = desc;
            this->desc->primitiveCount = std::min(this->desc->primitiveCount, uint32_t(buffer.range() / (buffer.stride() * 3ull))); // Make Bit Safer
        };
        virtual void setIndexBuffer(uint32_t indexBufferView = ~0u, VkIndexType indexType = VK_INDEX_TYPE_NONE_KHR) {
            this->desc->indexBufferView = indexBufferView, this->desc->indexType = indexType;
        };
        virtual void setVertexBuffer(uint32_t vertexAttribute = 0u){
            this->desc->vertexAttribute = vertexAttribute;
        };
    };

};
