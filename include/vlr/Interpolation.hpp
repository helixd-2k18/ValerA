#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./VertexSet.hpp"

namespace vlr {

    class Interpolation : public std::enable_shared_from_this<Interpolation> { protected: //using T = uint8_t;
        std::vector<uint32_t> attributeIDs = {};
        vkt::uni_ptr<VertexSet> vertexSet = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        Interpolation() { this->constructor(); };
        Interpolation(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {};

        // Call a Virtual Method
        virtual vkt::Vector<uint8_t> getBuffer_T(const uint32_t& I = 0u) const { return (*vertexSet)[attributeIDs[I]]; };
        virtual vkt::Vector<uint8_t> operator[](const uint32_t& I) const { return this->getBuffer_T(I); };
        
        // 
        template<class T = uint8_t>
        vkt::Vector<T> getBuffer(const uint32_t& I = 0u) const {
            return dynamic_cast<const vkt::Vector<T>&>(this->getBuffer_T(I));
        };

    };

};
