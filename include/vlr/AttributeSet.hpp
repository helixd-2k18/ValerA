#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./SetBase.hpp"

namespace vlr {

    using AttributeSet = SetBase_T<VkVertexInputAttributeDescription>;

};

namespace vlj {
    class AttributeSet : public Wrap<vlr::AttributeSet> {
    public:
        AttributeSet() : Wrap<vlr::AttributeSet>() {};
        AttributeSet(vkt::uni_ptr<vlr::AttributeSet> object) : Wrap<vlr::AttributeSet>(object) {};
        AttributeSet(vkt::uni_ptr<vlr::Driver> driver, vkt::uni_arg<vlr::DataSetCreateInfo> info = vlr::DataSetCreateInfo{}) : Wrap<vlr::AttributeSet>(std::make_shared<vlr::AttributeSet>(driver, info)) {};
        AttributeSet(std::shared_ptr<vlr::AttributeSet> object) : Wrap<vlr::AttributeSet>(object) {};

        //CALLIFY(constructor);
        CALLIFY(getCpuBuffer);
        CALLIFY(getGpuBuffer);
        //CALLIFY(get);
        CALLIFY(createDescriptorSet);
        CALLIFY(setCommand);
        CALLIFY(getVector);

        virtual VkVertexInputAttributeDescription* get(const uint32_t& I = 0u) { return &object->get(I); };
        virtual const VkVertexInputAttributeDescription* get(const uint32_t& I = 0u) const { return &object->get(I); };
    };
};
