#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./SetBase.hpp"

namespace vlr {

    using Constants = SetBase_T<ConstantDesc>;

};

namespace vlj {
    class Constants : public Wrap<vlr::Constants> {
    public:
        Constants() : Wrap<vlr::Constants>() {};
        Constants(vkt::uni_ptr<vlr::Constants> object) : Wrap<vlr::Constants>(object) {};
        Constants(vkt::uni_ptr<vlr::Driver> driver, vkt::uni_arg<vlr::DataSetCreateInfo> info = vlr::DataSetCreateInfo{}) : Wrap<vlr::Constants>(std::make_shared<vlr::Constants>(driver, info)) {};
        Constants(std::shared_ptr<vlr::Constants> object) : Wrap<vlr::Constants>(object) {};

        //CALLIFY(constructor);
        CALLIFY(getCpuBuffer);
        CALLIFY(getGpuBuffer);
        //CALLIFY(get);
        CALLIFY(createDescriptorSet);
        CALLIFY(setCommand);
        CALLIFY(getVector);
        //CALLIFY(getConstants);

        virtual vlr::ConstantDesc* get(const uint32_t& I = 0u) { return &object->get(I); };
        virtual const vlr::ConstantDesc* get(const uint32_t& I = 0u) const { return &object->get(I); };
        //CALLIFY(constructor);
    };
};
