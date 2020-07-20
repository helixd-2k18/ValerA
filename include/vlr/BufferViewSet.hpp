#pragma once
#include "./Config.hpp"

namespace vlr {

    class BufferViewSet : public std::enable_shared_from_this<BufferViewSet> { protected: 
        VkDescriptorSet set = {}; bool updated = false;
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};
        std::shared_ptr<Driver> driver = {};
        std::vector<vkt::Vector<uint8_t>> buffers = {};

    public: 
        BufferViewSet() { this->constructor(); };
        BufferViewSet(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {};
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier = false);
        virtual void pushBufferView(const vkt::uni_arg<vkt::Vector<uint8_t>>& bufferView) {
            this->buffers.push_back(bufferView);
        };
        virtual void resetBufferViews(){
            this->buffers.resize(0ull);
        };

        // 
        vkt::Vector<uint8_t>& operator[](const uint32_t& I) { return buffers[I]; };
        const vkt::Vector<uint8_t>& operator[](const uint32_t& I) const { return buffers[I]; };
    };

};
