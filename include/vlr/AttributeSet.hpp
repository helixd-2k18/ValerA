#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

    using AttributeSet = SetBase_T<VkVertexInputAttributeDescription>;

};

namespace vlj {
    class AttributeSet : public Wrap<vlr::AttributeSet> {
        AttributeSet() : Wrap<vlr::AttributeSet>() {};
        AttributeSet(vkt::uni_ptr<vlr::AttributeSet> object) : Wrap<vlr::AttributeSet>(object) {};
        AttributeSet(vkt::uni_ptr<vlr::Driver> driver, vkt::uni_arg<vlr::DataSetCreateInfo> info = {}) : Wrap<vlr::AttributeSet>(std::make_shared<vlr::AttributeSet>(driver, info)) {};

        //CALLIFY(constructor);
        CALLIFY(getCpuBuffer);
        CALLIFY(getGpuBuffer);
        CALLIFY(get);
        CALLIFY(createDescriptorSet);
        CALLIFY(setCommand);
        CALLIFY(getVector);
    };
};
