#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

namespace vlr {

#pragma pack(push, 1)
    struct MaterialUnit {
        glm::vec4 diffuse = glm::vec4(1.f);
        glm::vec4 pbrAGM = glm::vec4(0.f);
        glm::vec4 normal = glm::vec4(0.5f, 0.5f, 1.f, 1.f);
        glm::vec4 emission = glm::vec4(0.f);

        int diffuseTexture = -1;
        int pbrAGMTexture = -1;
        int normalTexture = -1;
        int emissionTexture = -1;

        glm::uvec4 udata = glm::uvec4(0u);
    };
#pragma pack(pop)

    using MaterialSet = SetBase_T<MaterialUnit>;

};

namespace vlj {
    class MaterialSet : public Wrap<vlr::MaterialSet> {
        MaterialSet() : Wrap<vlr::MaterialSet>() {};
        MaterialSet(vkt::uni_ptr<vlr::MaterialSet> object) : Wrap<vlr::MaterialSet>(object) {};
        MaterialSet(vkt::uni_ptr<vlr::Driver> driver, vkt::uni_arg<vlr::DataSetCreateInfo> info = {}) : Wrap<vlr::MaterialSet>(std::make_shared<vlr::MaterialSet>(driver, info)) {};
        MaterialSet(std::shared_ptr<vlr::MaterialSet> object) : Wrap<vlr::MaterialSet>(object) {};

        //CALLIFY(constructor);
        CALLIFY(getCpuBuffer);
        CALLIFY(getGpuBuffer);
        CALLIFY(get);
        CALLIFY(createDescriptorSet);
        CALLIFY(setCommand);
        CALLIFY(getVector);
    };
};
