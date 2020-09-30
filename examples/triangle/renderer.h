#pragma once

// 
#include "stdint.h"

// WIP "C" version for faster Neverball compilation, and more "C" compatibility (but can be used much slower C++)
#ifndef VLR_C_RENDERER
#define VLR_C_RENDERER

// 
struct CVertexData {
    float vertex[4];
    float normal[4];
    float texcoord[4];
};

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

struct CTextureSet {
    uint32_t ID; // Explicit Only
};

struct CSamplerSet {
    uint32_t ID; // Explicit Only
};

// Dirty hack for default initializer
void initMaterialUnit(CMaterialUnit* cunit);
void initVertexData(CVertexData* cunit);

#endif
