#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

    class BufferViewSet : public std::enable_shared_from_this<BufferViewSet> { protected: friend RayTracing; friend Rasterization; friend Resampling; friend PipelineLayout;
        VkDescriptorSet set = {}; bool updated = false, texelBuffer = false;
        std::vector<vkt::VectorBase> buffers = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        BufferViewSet() { this->constructor(); };
        
        BufferViewSet(vkt::uni_ptr<Driver> driver, const bool& texelBuffer = false) { this->constructor(driver, texelBuffer); };
        ~BufferViewSet() {
            const auto device = driver->getDeviceDispatch();
            if (set) { vkh::handleVk(device->vkFreeDescriptorSets(device->handle, driver->getDescriptorPool(), 1u, &set)); set = {}; };
        };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, const bool& texelBuffer = false) {
            this->driver = driver;
        };
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout);
        virtual void resetBufferViews(){ this->buffers.clear(); this->buffers.resize(0ull); };

        template<class T = uint8_t>
        intptr_t pushBufferView(vkt::uni_arg<vkt::Vector<T>> bufferView) {
            return this->pushBufferView(vkt::VectorBase(bufferView->getAllocation(), bufferView->offset(), bufferView->range(), bufferView->stride()));
        };

        virtual intptr_t pushBufferView(vkt::uni_arg<vkt::VectorBase> bufferView) {
            intptr_t ptr = this->buffers.size();
            this->buffers.push_back(bufferView);
            return ptr;
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
