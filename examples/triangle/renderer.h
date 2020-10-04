#pragma once

// 
#include "stdint.h"

// WIP "C" version for faster Neverball compilation, and more "C" compatibility (but can be used much slower C++)
#ifndef VLR_C_RENDERER
#define VLR_C_RENDERER

// 
//struct CVertexData {
//    float vertex[4];
//    float normal[4];
//    float texcoord[4];
//};

// You NOT able to use default parameters
// Even NOT able to use GLM
struct CMaterialUnit {
    float diffuse[4];
    float pbrAGM[4];
    float normal[4];
    float emission[4];

    int diffuseTexture;
    int pbrAGMTexture;
    int normalTexture;
    int emissionTexture;

    uint32_t udata[4];
};

// You NOT able to using operators
struct CMaterialSet {
    uint32_t ID; // Explicit Only
};

// 
struct CTextureSet {
    uint32_t ID; // Explicit Only
};

// 
struct CSamplerSet {
    uint32_t ID; // Explicit Only
};

// 
enum CFormat {
    CFLOAT = 0,
    CHALF = 1,
    CINT = 2
};

// 
enum CIndex {
    CNONE = 0,
    CUINT8 = 1,
    CUINT16 = 2,
    CUINT32 = 3
};

// 
struct CAttribLayout {
    uint32_t offset;
    uint32_t count;
    CFormat format;
};

//
struct CLayoutPreset {
    CAttribLayout vertex;
    CAttribLayout normal;
    CAttribLayout texcoord;
    CAttribLayout color;
};

// 
const CLayoutPreset NeverballLayoutPreset = {
    {.offset = 0u, .count = 3u, .format = CFLOAT },
    {.offset = 12u, .count = 3u, .format = CFLOAT },
    {.offset = 24u, .count = 2u, .format = CFLOAT },
    {.offset = 32u, .count = 0u, .format = CFLOAT }
};

// 
const CLayoutPreset LibLavaLayoutPreset = {
    {.offset = 0u, .count = 3u, .format = CFLOAT },
    {.offset = 36u, .count = 3u, .format = CFLOAT },
    {.offset = 28u, .count = 2u, .format = CFLOAT },
    {.offset = 12u, .count = 4u, .format = CFLOAT }
};

// Dirty hack for default initializer
void initMaterialUnit(CMaterialUnit* cunit);

// planned presets, such as LibLava, Neverball, Minecraft Chunks, Custom (for Minecraft again)
void initVertexLayout(uint32_t stride, CIndex indexType, CLayoutPreset preset);

#endif
