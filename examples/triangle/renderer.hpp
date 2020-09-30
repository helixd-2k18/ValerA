#pragma once

// 
#include <vkt3/core.hpp>
#include <glm/glm.hpp>

// Planned C++20 version of wrapper (faster C++ compilation time)
#ifndef VLR_CPP_RENDERER
#define VLR_CPP_RENDERER

// 
namespace vrp {

    // 
    struct VertexData {
        glm::vec4 vertex = glm::vec4(0.f, 0.f, 0.f, 1.f);
        glm::vec4 normal = glm::vec4(0.f, 1.f, 0.f, 0.f);
        glm::vec4 texcoord = glm::vec4(0.f, 0.f, 0.f, 0.f);
    };

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
class NAME {public: \
uint32_t ID = 0u;\
NAME(const uint32_t& id = 0u) { ID = id; };\
inline operator uint32_t& () {return ID;};\
inline operator const uint32_t& () const {return ID;};\
inline NAME& operator =(const uint32_t& id) {this->ID = id; return *this;};\
};

    // 
    TYPED(Base);

    // 
    class MaterialSet : public Base {
        MaterialSet(const uint32_t& id = 0u) : Base(id) {}

    };

    // 
    class TextureSet : public Base {
        TextureSet(const uint32_t& id = 0u) : Base(id) {}

    };

    // 
    class SamplerSet : public Base {
        SamplerSet(const uint32_t& id = 0u) : Base(id) {}

    };

};

#endif
