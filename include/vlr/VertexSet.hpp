#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./AttributeSet.hpp"
#include "./BindingSet.hpp"
#include "./BufferViewSet.hpp"

// 
namespace vlr {

    struct VertexSetCreateInfo {
        vkt::uni_ptr<BindingSet> bindings = {};
        vkt::uni_ptr<AttributeSet> attributes = {};
        vkt::uni_ptr<BufferViewSet> bufferViews = {};
    };

    class VertexSet : public std::enable_shared_from_this<VertexSet> { protected: friend RayTracing; friend Rasterization; friend PipelineLayout;
        vkt::uni_ptr<BufferViewSet> bufferViews = {};
        vkt::uni_ptr<BindingSet> bindings = {};
        vkt::uni_ptr<AttributeSet> attributes = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        VertexSet() { this->constructor(); };
        VertexSet(vkt::uni_ptr<Driver> driver, vkt::uni_arg<VertexSetCreateInfo> info = {}) { this->constructor(driver, info); };
        ~VertexSet() {};

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<VertexSetCreateInfo> info = {}) {
            this->driver = driver;
            this->bufferViews = info->bufferViews;
            this->attributes = info->attributes;
            this->bindings = info->bindings;
        };

        virtual vkh::VkVertexInputAttributeDescription& getAttribute(const uint32_t& I) { return dynamic_cast<vkt::Vector<vkh::VkVertexInputAttributeDescription>&>(this->attributes->getCpuBuffer())[I]; };
        virtual const vkh::VkVertexInputAttributeDescription& getAttribute(const uint32_t& I) const { return dynamic_cast<const vkt::Vector<vkh::VkVertexInputAttributeDescription>&>(this->attributes->getCpuBuffer())[I]; };

        virtual vkh::VkVertexInputBindingDescription& getBinding(const uint32_t& I) { return dynamic_cast<vkt::Vector<vkh::VkVertexInputBindingDescription>&>(this->bindings->getCpuBuffer())[I]; };
        virtual const vkh::VkVertexInputBindingDescription& getBinding(const uint32_t& I) const { return dynamic_cast<const vkt::Vector<vkh::VkVertexInputBindingDescription>&>(this->bindings->getCpuBuffer())[I]; };

        virtual const vkt::Vector<uint8_t>& operator[](const uint32_t& I) const { return this->getBuffer_T(I); };
        virtual vkt::Vector<uint8_t>& operator[](const uint32_t& I) { return this->getBuffer_T(I); };

        virtual const vkt::Vector<uint8_t>& getBuffer_T(const uint32_t& I) const { return (*this->bufferViews)[I]; };
        virtual vkt::Vector<uint8_t>& getBuffer_T(const uint32_t& I) { return (*this->bufferViews)[I]; };

        virtual vkt::Vector<uint8_t> getAttributeBuffer_T(const uint32_t& I) const {
            const vkh::VkVertexInputAttributeDescription attribute = dynamic_cast<const vkt::Vector<vkh::VkVertexInputAttributeDescription>&>(this->attributes->getCpuBuffer())[I];
            const vkh::VkVertexInputBindingDescription binding = dynamic_cast<const vkt::Vector<vkh::VkVertexInputBindingDescription>&>(this->bindings->getCpuBuffer())[attribute.binding];
            return (*this->bufferViews)[binding.binding];
        };

        template<class T = uint8_t>  // Call a Virtual Method
        const vkt::Vector<T>& getBuffer(const uint32_t& I = 0u) const { return dynamic_cast<const vkt::Vector<T>&>(this->getBuffer_T(I)); };

        template<class T = uint8_t>  // Call a Virtual Method
        const vkt::Vector<T>& getAttributeBuffer(const uint32_t& I = 0u) const { return dynamic_cast<const vkt::Vector<T>&>(this->getAttributeBuffer_T(I)); };

        // 
        void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
    };

};
