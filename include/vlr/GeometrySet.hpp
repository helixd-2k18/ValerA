#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./Geometry.hpp"

// 
namespace vlr {

    class GeometrySet : public SetBase_T<GeometryDesc> { protected: friend Acceleration; friend RayTracing; friend Rasterization;
        std::vector<vkt::uni_ptr<Geometry>> geometries = {};
        vkt::uni_ptr<VertexSet> vertexSet = {};

    public: 
        GeometrySet() : SetBase_T<GeometryDesc>() { this->constructorExtend0(); };
        GeometrySet(vkt::uni_ptr<VertexSet> vertexSet, vkt::uni_arg<DataSetCreateInfo> info = {}) : SetBase_T<GeometryDesc>(vertexSet->driver, info) { this->constructorExtend0(vertexSet, info); };
        ~GeometrySet() {};

        // 
        virtual void constructorExtend0() {};
        virtual void constructorExtend0(vkt::uni_ptr<VertexSet> vertexSet, vkt::uni_arg<DataSetCreateInfo> info = {}) {
            this->vertexSet = vertexSet;
        };
        virtual void pushGeometry(vkt::uni_ptr<Geometry> geometry);
        virtual void resetGeometries() { this->geometries.clear(); this->geometries.resize(0u); };

        // 
        virtual const vkt::VectorBase& getCpuBuffer() const override { return SetBase_T<GeometryDesc>::getCpuBuffer(); };
        virtual const vkt::VectorBase& getGpuBuffer() const override { return SetBase_T<GeometryDesc>::getGpuBuffer(); };
        virtual vkt::VectorBase& getCpuBuffer() override { return SetBase_T<GeometryDesc>::getCpuBuffer(); };
        virtual vkt::VectorBase& getGpuBuffer() override { return SetBase_T<GeometryDesc>::getGpuBuffer(); };

        // 
        virtual GeometryDesc& get(const uint32_t& I = 0u) override { return SetBase_T<GeometryDesc>::get(I); };
        virtual const GeometryDesc& get(const uint32_t& I = 0u) const override { return SetBase_T<GeometryDesc>::get(I); };

        // 
        GeometryDesc& operator[](const uint32_t& I) { return this->get(I); };
        const GeometryDesc& operator[](const uint32_t& I) const { return this->get(I); };

        // 
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) override;
    };

};

namespace vlj {
    class GeometrySet : public Wrap<vlr::GeometrySet> {
    public:
        GeometrySet() : Wrap<vlr::GeometrySet>() {};
        GeometrySet(vkt::uni_ptr<vlr::GeometrySet> object) : Wrap<vlr::GeometrySet>(object) {};
        GeometrySet(vkt::uni_ptr<vlr::VertexSet> vertexSet, vkt::uni_arg<vlr::DataSetCreateInfo> info = {}) : Wrap<vlr::GeometrySet>(std::make_shared<vlr::GeometrySet>(vertexSet, info)) {};
        GeometrySet(std::shared_ptr<vlr::GeometrySet> object) : Wrap<vlr::GeometrySet>(object) {};

        //CALLIFY(constructor);
        CALLIFY(getCpuBuffer);
        CALLIFY(getGpuBuffer);
        CALLIFY(get);
        CALLIFY(createDescriptorSet);
        CALLIFY(setCommand);
        CALLIFY(getVector);
    };
};

