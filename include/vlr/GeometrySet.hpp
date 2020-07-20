#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./SetBase.hpp"
#include "./Geometry.hpp"
#include "./BindingSet.hpp"
#include "./BufferViewSet.hpp"
#include "./Interpolation.hpp"

// 
namespace vlr {

    struct GeometryDesc {
        glm::mat3x4 transform = glm::mat3x4(1.f);
        uint32_t firstVertex = 0u;
        uint32_t primitiveCount = 0u;
        uint32_t material = 0u;
        uint32_t flags = 0u;
    };

    class GeometrySet : public SetBase_T<GeometryDesc> { protected: 
        std::vector<vkt::uni_ptr<Geometry>> geometries = {};
        std::vector<vkt::uni_ptr<Interpolation>> interpolations = {};
        vkt::uni_ptr<BufferViewSet> bufferViewSet = {};
        vkt::uni_ptr<BindingSet> bindings = {};
        vkt::uni_ptr<AttributeSet> attribute = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        GeometrySet() : SetBase_T<GeometryDesc>() { this->constructorExtend0(); };
        GeometrySet(vkt::uni_ptr<Driver> driver, vkt::uni_arg<DataSetCreateInfo> info) : SetBase_T<GeometryDesc>(driver, info) { this->constructorExtend0(driver); };

        virtual void constructorExtend0() {};
        virtual void constructorExtend0(vkt::uni_ptr<Driver> driver) {
            
        };

        virtual void resetGeometries() {
            interpolations.resize(0u);
            geometries.resize(0u);
        };

        virtual void pushGeometry(vkt::uni_ptr<Geometry> geometry, vkt::uni_ptr<Interpolation> interpolation, vkt::uni_ptr<GeometryDesc> desc) {
            uintptr_t I = this->getCpuBuffer().size();
            interpolations.push_back(interpolation);
            geometries.push_back(geometry);
            const vkh::VkVertexInputAttributeDescription attribute = dynamic_cast<vkt::Vector<vkh::VkVertexInputAttributeDescription>&>(*this->bindings)[geometry->vertexAttribute];
            const vkh::VkVertexInputBindingDescription binding = dynamic_cast<vkt::Vector<vkh::VkVertexInputBindingDescription>&>(*this->bindings)[attribute.binding];
            const vkt::Vector<uint8_t>& buffer = (*this->bufferViewSet)[binding.binding];
            {
                auto& geometryDesc = dynamic_cast<vkt::Vector<GeometryDesc>&>(this->getCpuBuffer());
                geometryDesc[I] = desc;
                geometryDesc[I].firstVertex += buffer.offset() / buffer.stride();
                geometryDesc[I].primitiveCount = std::min(geometryDesc[I].primitiveCount, uint32_t(buffer.range() / (buffer.stride() * 3ull))); // Make Bit Safer
            };
        };
    };

};
