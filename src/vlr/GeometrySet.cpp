#include "./vlr/GeometrySet.hpp"
#include "./vlr/Geometry.hpp"
#include "./vlr/Interpolation.hpp"
#include "./vlr/VertexSet.hpp"
#include "./vlr/SetBase.hpp"

namespace vlr {

    void GeometrySet::pushGeometry(vkt::uni_ptr<Geometry> geometry, vkt::uni_ptr<Interpolation> interpolation) {
        uintptr_t I = this->getCpuBuffer().size();
        interpolations.push_back(interpolation);
        geometries.push_back(geometry);

        vkt::Vector<uint8_t> buffer = this->vertexSet->getBuffer_T(geometry->vertexAttribute);
        {
            vkt::Vector<GeometryDesc>& geometryDesc = dynamic_cast<vkt::Vector<GeometryDesc>&>(this->getCpuBuffer());
            geometryDesc[I] = geometry->desc;
            geometryDesc[I].primitiveCount = std::min(geometryDesc[I].primitiveCount, uint32_t(buffer.range() / (buffer.stride() * 3ull))); // Make Bit Safer
        };
    };

};
