#include "./vlr/GeometrySet.hpp"
#include "./vlr/Geometry.hpp"
#include "./vlr/VertexSet.hpp"

namespace vlr {

    void GeometrySet::pushGeometry(vkt::uni_ptr<Geometry> geometry) {
        uintptr_t I = geometries.size();//this->getCpuBuffer().size();
        geometries.push_back(geometry);

        vkt::VectorBase buffer = this->vertexSet->getAttributeBuffer_T(geometry->desc.vertexAttribute);
        {
            vkt::Vector<GeometryDesc>& geometryDesc = dynamic_cast<vkt::Vector<GeometryDesc>&>(this->getCpuBuffer());
            geometryDesc[I] = geometry->desc;
        };
    };

    void GeometrySet::createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) {
        SetBase_T<GeometryDesc>::createDescriptorSet(pipelineLayout);
        vertexSet->createDescriptorSet(pipelineLayout);

        for (uint32_t i=0;i<geometries.size();i++) {
            vkt::uni_ptr<Geometry> geometry = geometries[i];
            vkt::VectorBase buffer = this->vertexSet->getAttributeBuffer_T(geometry->desc.vertexAttribute);
            {
                vkt::Vector<GeometryDesc>& geometryDesc = dynamic_cast<vkt::Vector<GeometryDesc>&>(this->getCpuBuffer());
                geometryDesc[i] = geometry->desc;
            };
        };
    };

};
