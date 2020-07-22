#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

    class BufferViewSet : public std::enable_shared_from_this<BufferViewSet> { protected: friend RayTracing; friend Rasterization; friend Resampling;
        VkDescriptorSet set = {}; bool updated = false;
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};
        std::vector<vkt::Vector<uint8_t>> buffers = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        BufferViewSet() { this->constructor(); };
        BufferViewSet(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {};
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier = false);
        virtual void pushBufferView_T(const vkt::uni_arg<vkt::Vector<uint8_t>>& bufferView) { this->buffers.push_back(bufferView); };
        virtual void resetBufferViews(){ this->buffers.resize(0ull); };

        template<class T = uint8_t>
        void pushBufferView(vkt::uni_arg<vkt::Vector<T>> bufferView) {
            this->pushBufferView_T(dynamic_cast<vkt::Vector<uint8_t>&>(*bufferView));
        };

        virtual void pushBufferView(vkt::uni_arg<vkt::VectorBase> bufferView) {
            this->pushBufferView_T(dynamic_cast<vkt::Vector<uint8_t>&>(*bufferView));
        };

        // 
        vkt::Vector<uint8_t>& operator[](const uint32_t& I) { return buffers[I]; };
        const vkt::Vector<uint8_t>& operator[](const uint32_t& I) const { return buffers[I]; };
    };

};
