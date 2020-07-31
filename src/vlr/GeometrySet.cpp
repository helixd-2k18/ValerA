#include "./vlr/GeometrySet.hpp"
#include "./vlr/Geometry.hpp"
#include "./vlr/Interpolation.hpp"
#include "./vlr/VertexSet.hpp"

namespace vlr {

    void GeometrySet::setInterpolation(vkt::uni_ptr<Interpolation> interpolation) {
        this->interpolations = interpolation;
    };

    void GeometrySet::pushGeometry(vkt::uni_ptr<Geometry> geometry) {
        uintptr_t I = geometries.size();//this->getCpuBuffer().size();
        geometries.push_back(geometry);

        vkt::Vector<uint8_t> buffer = this->vertexSet->getBuffer_T(geometry->desc->vertexAttribute);
        {
            vkt::Vector<GeometryDesc>& geometryDesc = dynamic_cast<vkt::Vector<GeometryDesc>&>(this->getCpuBuffer());
            geometryDesc[I] = geometry->desc;
            geometryDesc[I].primitiveCount = std::min(geometryDesc[I].primitiveCount, uint32_t(buffer.range() / (buffer.stride() * 3ull))); // Make Bit Safer
        };
    };

    void GeometrySet::createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) {
        SetBase_T<GeometryDesc>::createDescriptorSet(pipelineLayout);
        vertexSet->createDescriptorSet(pipelineLayout);

        for (uint32_t i=0;i<geometries.size();i++) {
            vkt::uni_ptr<Geometry> geometry = geometries[i];
            vkt::uni_ptr<Interpolation> interpolation = interpolations[i];
            
            //interpBufs->pushBufferView(interpolation->getGpuBuffer());
        };
    };

};
