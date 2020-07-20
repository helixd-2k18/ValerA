#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./vlr/AttributeSet.hpp"
#include "./vlr/BindingSet.hpp"
#include "./vlr/BufferViewSet.hpp"

// 
namespace vlr {

    class VertexSet : public std::enable_shared_from_this<Interpolation> { protected: 
        vkt::uni_ptr<BufferViewSet> bufferViews = {};
        vkt::uni_ptr<BindingSet> bindings = {};
        vkt::uni_ptr<AttributeSet> attributes = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        VertexSet() { this->constructor(); };
        VertexSet(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };
        
        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            
        };

        virtual vkt::Vector<uint8_t> operator[](const uint32_t& I) const { return this->getBuffer_T(I); };
        virtual vkt::Vector<uint8_t> getBuffer_T(const uint32_t& I) const {
            const vkh::VkVertexInputAttributeDescription attribute = dynamic_cast<const vkt::Vector<vkh::VkVertexInputAttributeDescription>&>(this->attributes->getCpuBuffer())[I];
            const vkh::VkVertexInputBindingDescription binding = dynamic_cast<const vkt::Vector<vkh::VkVertexInputBindingDescription>&>(this->bindings->getCpuBuffer())[attribute.binding];
            return (*this->bufferViews)[binding.binding];
        };

        template<class T = uint8_t>
        vkt::Vector<T> getBuffer(const uint32_t& I = 0u) const {
            return dynamic_cast<const vkt::Vector<T>&>(this->getBuffer_T(I)); // Call a Virtual Method
        };
    };

};
