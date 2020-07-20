#include "./vlr/GeometrySet.hpp"
#include "./vlr/SetBase.hpp"
#include "./vlr/Geometry.hpp"
#include "./vlr/BindingSet.hpp"
#include "./vlr/BufferViewSet.hpp"
#include "./vlr/Interpolation.hpp"

namespace vlr {

    void GeometrySet::pushGeometry(vkt::uni_ptr<Geometry> geometry, vkt::uni_ptr<Interpolation> interpolation, vkt::uni_ptr<GeometryDesc> desc) {
        uintptr_t I = this->getCpuBuffer().size();
        interpolations.push_back(interpolation);
        geometries.push_back(geometry);

        vkt::Vector<uint8_t> buffer = this->vertexSet->getBuffer_T(geometry->vertexAttribute);
        {
            auto& geometryDesc = dynamic_cast<vkt::Vector<GeometryDesc>&>(this->getCpuBuffer());
            geometryDesc[I] = desc;
            geometryDesc[I].firstVertex += buffer.offset() / buffer.stride();
            geometryDesc[I].primitiveCount = std::min(geometryDesc[I].primitiveCount, uint32_t(buffer.range() / (buffer.stride() * 3ull))); // Make Bit Safer
        };
    };

};
