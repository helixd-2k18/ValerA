#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./VertexSet.hpp"

namespace vlr {

#pragma pack(push, 1)
    struct InterpData {
        uint32_t data[8u];
    };
#pragma pack(pop)

    // Now contains multiple geometries
    class Interpolation : public SetBase_T<InterpData>  { protected: //using T = uint8_t;
        vkt::uni_ptr<VertexSet> vertexSet = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        Interpolation() : SetBase_T<InterpData>() { this->constructorExtend0(); };
        Interpolation(vkt::uni_ptr<Driver> driver, vkt::uni_arg<DataSetCreateInfo> info) : SetBase_T<InterpData>(driver, info) { this->constructorExtend0(driver); };

        // 
        virtual void constructorExtend0() {};
        virtual void constructorExtend0(vkt::uni_ptr<Driver> driver) {};

        // Call a Virtual Method
        virtual vkt::Vector<uint8_t> getAttributeBuffer_T(const uint32_t& I = 0u, const uint32_t& G = 0u) const { return vertexSet->getAttributeBuffer(this->get(G).data[I]); };

        // 
        template<class T = uint8_t>
        vkt::Vector<T> getAttributeBuffer(const uint32_t& I = 0u, const uint32_t& G = 0u) const {
            return dynamic_cast<const vkt::Vector<T>&>(this->getAttributeBuffer_T(G, I));
        };

        // 
        virtual const vkt::VectorBase& getCpuBuffer() const override { return SetBase_T<InterpData>::getCpuBuffer(); };
        virtual const vkt::VectorBase& getGpuBuffer() const override { return SetBase_T<InterpData>::getGpuBuffer(); };
        virtual vkt::VectorBase& getCpuBuffer() override { return SetBase_T<InterpData>::getCpuBuffer(); };
        virtual vkt::VectorBase& getGpuBuffer() override { return SetBase_T<InterpData>::getGpuBuffer(); };

        // 
        virtual InterpData& get(const uint32_t& I = 0u) override { return SetBase_T<InterpData>::get(I); };
        virtual const InterpData& get(const uint32_t& I = 0u) const override { return SetBase_T<InterpData>::get(I); };

        // 
        InterpData& operator[](const uint32_t& I) { return this->get(I); };
        const InterpData& operator[](const uint32_t& I) const { return this->get(I); };

    };

};
