#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./SetBase.hpp"

namespace vlr {

    using MaterialSet = SetBase_T<MaterialUnit>;

};

namespace vlj {
    class MaterialSet : public Wrap<vlr::MaterialSet> {
    public:
        MaterialSet() : Wrap<vlr::MaterialSet>() {};
        MaterialSet(vkt::uni_ptr<vlr::MaterialSet> object) : Wrap<vlr::MaterialSet>(object) {};
        MaterialSet(vkt::uni_ptr<vlr::Driver> driver, vkt::uni_arg<vlr::DataSetCreateInfo> info = {}) : Wrap<vlr::MaterialSet>(std::make_shared<vlr::MaterialSet>(driver, info)) {};
        MaterialSet(std::shared_ptr<vlr::MaterialSet> object) : Wrap<vlr::MaterialSet>(object) {};

        //CALLIFY(constructor);
        CALLIFY(getCpuBuffer);
        CALLIFY(getGpuBuffer);
        //CALLIFY(get);
        CALLIFY(createDescriptorSet);
        CALLIFY(setCommand);
        CALLIFY(getVector);

        virtual vlr::MaterialUnit* get(const uint32_t& I = 0u) { return &object->get(I); };
        virtual const vlr::MaterialUnit* get(const uint32_t& I = 0u) const { return &object->get(I); };
    };
};
