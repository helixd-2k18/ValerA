#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./SetBase.hpp"
#include "./Geometry.hpp"
#include "./BindingSet.hpp"
#include "./BufferViewSet.hpp"

namespace vlr {

    class Interpolation : public std::enable_shared_from_this<Interpolation> { protected: //using T = uint8_t;
        std::vector<uint32_t> bindingIDs = {};
        vkt::uni_ptr<BufferViewSet> bufferViewSet = {};
        vkt::uni_ptr<BindingSet> bindings = {};
        vkt::uni_ptr<AttributeSet> attribute = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        Interpolation() { this->constructor(); };
        Interpolation(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };

        virtual vkt::Vector<uint8_t> getBuffer(const uint32_t& I = 0u) {
            const uint32_t bindingID = bindingIDs[I];
            const vkh::VkVertexInputBindingDescription binding = dynamic_cast<const vkt::Vector<vkh::VkVertexInputBindingDescription>&>(*this->bindings)[bindingID];
            return (*this->bufferViewSet)[binding.binding];
        };

        template<class T = uint8_t>
        vkt::Vector<T> getBuffer(const uint32_t& I = 0u) const {
            return dynamic_cast<const vkt::Vector<T>&>(this->getBuffer(I)); // Call a Virtual Method
        };

        // 
        vkt::Vector<uint8_t>& operator[](const uint32_t& I) { return this->getBuffer(I); };
        const vkt::Vector<uint8_t>& operator[](const uint32_t& I) const { return this->getBuffer(I); };
    };

};
