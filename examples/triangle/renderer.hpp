#pragma once

// 
#include <vkt3/core.hpp>
#include <glm/glm.hpp>

// Planned C++20 version of wrapper (faster C++ compilation time)
#ifndef VLR_CPP_RENDERER
#define VLR_CPP_RENDERER

// 
namespace vrp {

    // usable by C++20
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

// type-safe identifiers
#define TYPED(NAME) \
struct NAME {\
uint32_t ID = 0u;\
NAME(uint32_t id = 0u) { ID = id; };\
inline operator uint32_t& () {return ID;};\
inline operator const uint32_t& () const {return ID;};\
inline NAME& operator =(const uint32_t& id) {this->ID = id; return *this;};\
};

    TYPED(CMaterialSet);
    TYPED(CTextureSet);
    TYPED(CSamplerSet);

};

#endif
