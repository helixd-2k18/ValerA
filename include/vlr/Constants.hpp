#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

#pragma pack(push, 1)
    struct ConstantDesc {
        glm::mat4 projection = glm::mat4(1.f);
        glm::mat4 projectionInv = glm::mat4(1.f);
        glm::mat3x4 modelview = glm::mat3x4(1.f);
        glm::mat3x4 modelviewInv = glm::mat3x4(1.f);
        glm::mat3x4 modelviewPrev = glm::mat3x4(1.f);
        glm::mat3x4 modelviewPrevInv = glm::mat3x4(1.f);
        glm::uvec4 mdata = glm::uvec4(0u);                         // mesh mutation or modification data
        //glm::uvec2 tdata = glm::uvec2(0u), rdata = glm::uvec2(0u); // first for time, second for randoms
        glm::uvec2 tdata = glm::uvec2(0u);
        glm::uvec2 rdata = glm::uvec2(0u);
    };
#pragma pack(pop)

    using Constants = SetBase_T<ConstantDesc>;

};

namespace vlj {
    class Constants : public Wrap<vlr::Constants> {
        Constants() : Wrap<vlr::Constants>() {};
        Constants(vkt::uni_ptr<vlr::Constants> object) : Wrap<vlr::Constants>(object) {};
        Constants(vkt::uni_ptr<vlr::Driver> driver, vkt::uni_arg<vlr::DataSetCreateInfo> info = {}) : Wrap<vlr::Constants>(std::make_shared<vlr::Constants>(driver, info)) {};
        Constants(std::shared_ptr<vlr::Constants> object) : Wrap<vlr::Constants>(object) {};

        //CALLIFY(constructor);
    };
};
