#pragma once
#include "./Config.hpp"

namespace vlr {

    struct MaterialUnit {
        glm::vec4 diffuse = glm::vec4(1.f);
        glm::vec4 specular = glm::vec4(1.f);
    };

    using MaterialSet = SetBase_T<MaterialUnit>;

};
