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

    class VertexSet : public std::enable_shared_from_this<VertexSet> {
    protected: friend RayTracing; friend Rasterization; friend PipelineLayout; friend GeometrySet; friend Geometry;
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

        virtual vkh::VkVertexInputAttributeDescription& getAttribute(const uint32_t& I) { 
            auto buffer = this->attributes->getCpuBuffer();
            auto vector = vkt::Vector<vkh::VkVertexInputAttributeDescription>(buffer.getAllocation(), buffer.offset(), buffer.range(), buffer.stride());
            return vector[I];
        };
        virtual const vkh::VkVertexInputAttributeDescription& getAttribute(const uint32_t& I) const {
            auto buffer = this->attributes->getCpuBuffer();
            auto vector = vkt::Vector<vkh::VkVertexInputAttributeDescription>(buffer.getAllocation(), buffer.offset(), buffer.range(), buffer.stride());
            return vector[I];
        };

        virtual vkh::VkVertexInputBindingDescription& getBinding(const uint32_t& I) { 
            auto buffer = this->bindings->getCpuBuffer();
            auto vector = vkt::Vector<vkh::VkVertexInputBindingDescription>(buffer.getAllocation(), buffer.offset(), buffer.range(), buffer.stride());
            return vector[I];
        };
        virtual const vkh::VkVertexInputBindingDescription& getBinding(const uint32_t& I) const {
            auto buffer = this->bindings->getCpuBuffer();
            auto vector = vkt::Vector<vkh::VkVertexInputBindingDescription>(buffer.getAllocation(), buffer.offset(), buffer.range(), buffer.stride());
            return vector[I];
        };

        virtual const vkt::VectorBase& operator[](const uint32_t& I) const { return this->getBuffer_T(I); };
        virtual vkt::VectorBase& operator[](const uint32_t& I) { return this->getBuffer_T(I); };

        virtual const vkt::VectorBase& getBuffer_T(const uint32_t& I) const { return (*this->bufferViews)[I]; };
        virtual vkt::VectorBase& getBuffer_T(const uint32_t& I) { return (*this->bufferViews)[I]; };

        virtual vkt::VectorBase getAttributeBuffer_T(const uint32_t& I) const {
            const vkh::VkVertexInputAttributeDescription attribute = this->getAttribute(I);
            const vkh::VkVertexInputBindingDescription binding = this->getBinding(attribute.binding);
            return (*this->bufferViews)[binding.binding];
        };

        template<class T = uint8_t>  // Call a Virtual Method
        vkt::Vector<T> getBuffer(const uint32_t& I = 0u) const { 
            auto buffer = this->getBuffer_T(I);
            return vkt::Vector<T>(buffer.getAllocation(), buffer.offset(), buffer.range(), buffer.stride());
        };

        template<class T = uint8_t>  // Call a Virtual Method
        vkt::Vector<T> getAttributeBuffer(const uint32_t& I = 0u) const { 
            auto buffer = this->getAttributeBuffer_T(I);
            return vkt::Vector<T>(buffer.getAllocation(), buffer.offset(), buffer.range(), buffer.stride());
        };

        // 
        void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
    };

};

namespace vlj {
    class VertexSet : public Wrap<vlr::VertexSet> {
    public:
        VertexSet() : Wrap<vlr::VertexSet>() {};
        VertexSet(vkt::uni_ptr<vlr::VertexSet> object) : Wrap<vlr::VertexSet>(object) {};
        VertexSet(vkt::uni_ptr<vlr::Driver> driver, vkt::uni_arg<vlr::VertexSetCreateInfo> info = {}) : Wrap<vlr::VertexSet>(std::make_shared<vlr::VertexSet>(driver, info)) {};
        VertexSet(std::shared_ptr<vlr::VertexSet> object) : Wrap<vlr::VertexSet>(object) {};

        //CALLIFY(constructor);
        CALLIFY(getAttribute);
        CALLIFY(getBinding);
        CALLIFY(getBuffer_T);
        CALLIFY(getAttributeBuffer_T);
        CALLIFY(getBuffer);
        CALLIFY(getAttributeBuffer);
    };
};
