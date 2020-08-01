#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

    class BufferViewSet : public std::enable_shared_from_this<BufferViewSet> { protected: friend RayTracing; friend Rasterization; friend Resampling; friend PipelineLayout;
        VkDescriptorSet set = {}; bool updated = false;
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};
        std::vector<vkt::Vector<uint8_t>> buffers = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        BufferViewSet() { this->constructor(); };
        BufferViewSet(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };
        ~BufferViewSet() {};

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            this->driver = driver;
        };
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        //virtual void setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier = false);
        virtual void pushBufferView_T(const vkt::uni_arg<vkt::Vector<uint8_t>>& bufferView) { this->buffers.push_back(bufferView); };
        virtual void resetBufferViews(){ this->buffers.resize(0ull); };

        template<class T = uint8_t>
        void pushBufferView(vkt::uni_arg<vkt::Vector<T>> bufferView) {
            this->pushBufferView_T(vkt::Vector<uint8_t>(bufferView->getAllocation(), bufferView->offset(), bufferView->range(), bufferView->stride()));
        };

        virtual void pushBufferView(vkt::uni_arg<vkt::VectorBase> bufferView) {
            this->pushBufferView_T(vkt::Vector<uint8_t>(bufferView->getAllocation(), bufferView->offset(), bufferView->range(), bufferView->stride()));
        };

        // 
        vkt::Vector<uint8_t>& get(const uint32_t& I = 0u) { return buffers[I]; };
        const vkt::Vector<uint8_t>& get(const uint32_t& I = 0u) const { return buffers[I]; };

        // 
        vkt::Vector<uint8_t>& operator[](const uint32_t& I) { return get(I); };
        const vkt::Vector<uint8_t>& operator[](const uint32_t& I) const { return get(I); };
    };

};
