#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

    class BufferViewSet : public std::enable_shared_from_this<BufferViewSet> { protected: friend RayTracing; friend Rasterization; friend Resampling; friend PipelineLayout;
        VkDescriptorSet set = {}; bool updated = false, texelBuffer = false;
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};
        std::vector<vkt::VectorBase> buffers = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        BufferViewSet() { this->constructor(); };
        
        BufferViewSet(vkt::uni_ptr<Driver> driver, const bool& texelBuffer = false) { this->constructor(driver, texelBuffer); };
        ~BufferViewSet() {};

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, const bool& texelBuffer = false) {
            this->driver = driver;
        };
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        //virtual void setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier = false);
        virtual void pushBufferView_T(const vkt::uni_arg<vkt::Vector<uint8_t>>& bufferView) { this->buffers.push_back(vkt::VectorBase(bufferView->getAllocation(), bufferView->offset(), bufferView->range(), bufferView->stride())); };
        virtual void pushBufferView_T(const vkt::uni_arg<vkt::VectorBase>& bufferView) { this->buffers.push_back(bufferView); };
        virtual void resetBufferViews(){ this->buffers.clear(); this->buffers.resize(0ull); };

        template<class T = uint8_t>
        void pushBufferView(vkt::uni_arg<vkt::Vector<T>> bufferView) {
            this->pushBufferView_T(vkt::VectorBase(bufferView->getAllocation(), bufferView->offset(), bufferView->range(), bufferView->stride()));
        };

        virtual void pushBufferView(vkt::uni_arg<vkt::VectorBase> bufferView) {
            this->pushBufferView_T(bufferView);
        };

        // 
        vkt::VectorBase& get(const uint32_t& I = 0u) {
            if (buffers.size() <= I) { buffers.resize(I+1); }; return buffers[I];
        };
        const vkt::VectorBase& get(const uint32_t& I = 0u) const { 
            //if (buffers.size() <= I) { buffers.resize(I + 1); }; return buffers[I];
            return buffers[I];
        };

        // 
        vkt::VectorBase& operator[](const uint32_t& I) { return get(I); };
        const vkt::VectorBase& operator[](const uint32_t& I) const { return get(I); };
    };

};

namespace vlj {
    class BufferViewSet : public Wrap<vlr::BufferViewSet> {
    public:
        BufferViewSet() : Wrap<vlr::BufferViewSet>() {};
        BufferViewSet(vkt::uni_ptr<vlr::BufferViewSet> object) : Wrap<vlr::BufferViewSet>(object) {};
        BufferViewSet(vkt::uni_ptr<vlr::Driver> driver, const bool& texelBuffer = false) : Wrap<vlr::BufferViewSet>(std::make_shared<vlr::BufferViewSet>(driver, texelBuffer)) {};
        BufferViewSet(std::shared_ptr<vlr::BufferViewSet> object) : Wrap<vlr::BufferViewSet>(object) {};

        //CALLIFY(constructor);
        CALLIFY(pushBufferView);
        CALLIFY(resetBufferViews);
        CALLIFY(createDescriptorSet);
        CALLIFY(get);
    };
};
