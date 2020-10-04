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


    // formats
    enum Format : uint32_t {
        FLOAT = 0,
        HALF = 1,
        INT = 2
    };

    // indices
    enum Index : uint32_t {
        NONE = 0,
        UINT8 = 1,
        UINT16 = 2,
        UINT32 = 3
    };

    // attributes
    struct AttribLayout {
        uint32_t offset = 0u;
        uint32_t count = 0u;
        Format format = Format::FLOAT;
    };

    // defaultly Neverball layout
    struct LayoutPreset {
        AttribLayout vertex = { .offset = 0u, .count = 3u, .format = Format::FLOAT };
        AttribLayout normal = { .offset = 12u, .count = 3u, .format = Format::FLOAT };
        AttribLayout texcoord = { .offset = 24u, .count = 3u, .format = Format::FLOAT };
        AttribLayout color = { .offset = 24u, .count = 0u, .format = Format::FLOAT };
    };

    // 
    const LayoutPreset NeverballLayoutPreset = {
        {.offset = 0u, .count = 3u, .format = Format::FLOAT },
        {.offset = 12u, .count = 3u, .format = Format::FLOAT },
        {.offset = 24u, .count = 2u, .format = Format::FLOAT },
        {.offset = 32u, .count = 0u, .format = Format::FLOAT }
    };

    // 
    const LayoutPreset LibLavaLayoutPreset = {
        {.offset = 0u, .count = 3u, .format = Format::FLOAT },
        {.offset = 36u, .count = 3u, .format = Format::FLOAT },
        {.offset = 28u, .count = 2u, .format = Format::FLOAT },
        {.offset = 12u, .count = 4u, .format = Format::FLOAT }
    };

    // planned presets, such as LibLava, Neverball, Minecraft Chunks, Custom (for Minecraft again)
    void initVertexLayout(uint32_t stride, Index indexType, LayoutPreset preset);

};

#endif
