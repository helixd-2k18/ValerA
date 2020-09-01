#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./SetBase.hpp"

namespace vlr {

    using BindingSet = SetBase_T<VkVertexInputBindingDescription>;

};

namespace vlj {
    class BindingSet : public Wrap<vlr::BindingSet> {
    public:
        BindingSet() : Wrap<vlr::BindingSet>() {};
        BindingSet(vkt::uni_ptr<vlr::BindingSet> object) : Wrap<vlr::BindingSet>(object) {};
        BindingSet(vkt::uni_ptr<vlr::Driver> driver, vkt::uni_arg<vlr::DataSetCreateInfo> info = vlr::DataSetCreateInfo{}) : Wrap<vlr::BindingSet>(std::make_shared<vlr::BindingSet>(driver, info)) {};
        BindingSet(std::shared_ptr<vlr::BindingSet> object) : Wrap<vlr::BindingSet>(object) {};

        //CALLIFY(constructor);
        CALLIFY(getCpuBuffer);
        CALLIFY(getGpuBuffer);
        //CALLIFY(get);
        CALLIFY(createDescriptorSet);
        CALLIFY(setCommand);
        CALLIFY(getVector);

        virtual VkVertexInputBindingDescription* get(const uint32_t& I = 0u) { return &object->get(I); };
        virtual const VkVertexInputBindingDescription* get(const uint32_t& I = 0u) const { return &object->get(I); };
    };
};
