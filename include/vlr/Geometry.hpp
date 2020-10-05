#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./VertexSet.hpp"

namespace vlr {



    class Geometry : public std::enable_shared_from_this<Geometry> { protected: friend GeometrySet; friend Acceleration; friend Rasterization;
        vkt::uni_ptr<VertexSet> vertexSet = {};
        GeometryDesc desc = {};

    public: 
        Geometry() { this->constructor(); };
        Geometry(vkt::uni_ptr<VertexSet> vertexSet, vkt::uni_arg<GeometryDesc> desc = {}) { this->constructor(vertexSet, desc); };
        ~Geometry() {};

        // 
        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<VertexSet> vertexSet, vkt::uni_arg<GeometryDesc> desc = {}) {
            this->vertexSet = vertexSet, this->desc = desc;
            //auto buffer = this->vertexSet->getAttributeBuffer_T(this->desc->vertexAttribute);
            //this->desc->primitiveCount = std::min(this->desc->primitiveCount, uint32_t(buffer.range() / (buffer.stride() * 3ull))); // Make Bit Safer
        };
        virtual void setIndexBuffer(uint32_t indexBufferView = ~0u, VkIndexType indexType = VK_INDEX_TYPE_NONE_KHR) {
            this->desc.indexBufferView = indexBufferView, this->desc.indexType = indexType;
        };
        virtual void setVertexBuffer(uint32_t vertexAttribute = 0u){
            this->desc.vertexAttribute = vertexAttribute;
        };

        // 
        GeometryDesc& getDesc() { return desc; };
        const GeometryDesc& getDesc() const { return desc; };
    };

};

namespace vlj {
    class Geometry : public Wrap<vlr::Geometry> {
    public:
        Geometry() : Wrap<vlr::Geometry>() {};
        Geometry(vkt::uni_ptr<vlr::Geometry> object) : Wrap<vlr::Geometry>(object) {};
        Geometry(vkt::uni_ptr<vlr::VertexSet> vertexSet, vkt::uni_arg<vlr::GeometryDesc> desc = {}) : Wrap<vlr::Geometry>(std::make_shared<vlr::Geometry>(vertexSet, desc)) {};
        Geometry(std::shared_ptr<vlr::Geometry> object) : Wrap<vlr::Geometry>(object) {};

        //CALLIFY(constructor);
        CALLIFY(setIndexBuffer);
        CALLIFY(setVertexBuffer);

        // 
        vlr::GeometryDesc* getDesc() { return &object->getDesc(); };
        const vlr::GeometryDesc* getDesc() const { return &object->getDesc(); };
    };
};
