#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./VertexSet.hpp"

namespace vlr {

    class Interpolation : public SetBase_T<uint32_t>  { protected: //using T = uint8_t;
        vkt::uni_ptr<VertexSet> vertexSet = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        Interpolation() : SetBase_T<uint32_t>() { this->constructorExtend0(); };
        Interpolation(vkt::uni_ptr<Driver> driver, vkt::uni_arg<DataSetCreateInfo> info) : SetBase_T<uint32_t>(driver, info) { this->constructorExtend0(driver); };

        // 
        virtual void constructorExtend0() {};
        virtual void constructorExtend0(vkt::uni_ptr<Driver> driver) {};

        // Call a Virtual Method
        virtual vkt::Vector<uint8_t> getBuffer_T(const uint32_t& I = 0u) const { return vertexSet->getAttributeBuffer(this->get(I)); };
        virtual vkt::Vector<uint8_t> operator[](const uint32_t& I) const { return this->getBuffer_T(I); };

        // 
        template<class T = uint8_t>
        vkt::Vector<T> getBuffer(const uint32_t& I = 0u) const {
            return dynamic_cast<const vkt::Vector<T>&>(this->getBuffer_T(I));
        };

        // 
        virtual const vkt::VectorBase& getCpuBuffer() const override { return SetBase_T<uint32_t>::getCpuBuffer(); };
        virtual const vkt::VectorBase& getGpuBuffer() const override { return SetBase_T<uint32_t>::getGpuBuffer(); };
        virtual vkt::VectorBase& getCpuBuffer() override { return SetBase_T<uint32_t>::getCpuBuffer(); };
        virtual vkt::VectorBase& getGpuBuffer() override { return SetBase_T<uint32_t>::getGpuBuffer(); };

        // 
        virtual uint32_t& get(const uint32_t& I = 0u) override { return SetBase_T<uint32_t>::get(I); };
        virtual const uint32_t& get(const uint32_t& I = 0u) const override { return SetBase_T<uint32_t>::get(I); };

        // 
        uint32_t& operator[](const uint32_t& I) { return this->get(I); };
        //const uint32_t& operator[](const uint32_t& I) const { return this->get(I); };

    };

};
