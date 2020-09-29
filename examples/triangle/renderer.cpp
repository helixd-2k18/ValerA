#pragma once

// 
#include "./stdafx.h"
//#include "renderdoc_app.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vec_swizzle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

// C++ sideway (static data only)
// C++ and C code can be mixed!
namespace vrc {
    inline static vlj::Driver driver = {};
    inline static uint32_t instanceCount = 0u;

    //
    inline static std::vector<vlj::BufferViewSet> bufferViews = {};
    inline static std::vector<vlj::AttributeSet> attributeSet = {};
    inline static std::vector<vlj::BindingSet> bindingSet = {};

    //
    inline static std::vector<vlj::VertexSet> vertexSet = {};
    inline static std::vector<vlj::Geometry> geometries = {};
    inline static std::vector<vlj::GeometrySet> geometrySets = {};
    inline static std::vector<vlj::Acceleration> accelerations = {};
    inline static vlj::InstanceSet instanceSet = {};
    inline static vlj::Acceleration accelerationTop = {};

    //
    inline static vlj::MaterialSet materialSet = {};
    inline static vlj::TextureSet textureSet = {};
    inline static vlj::SamplerSet samplerSet = {};

    // data slots
    inline static std::vector<uint32_t> availableTextures = {};
    inline static std::vector<uint32_t> availableSamplers = {};
    inline static std::vector<uint32_t> availableMaterials = {};

    // geometry slots
    inline static std::vector<uint32_t> availableGeometries = {};
    inline static std::vector<uint32_t> availableGeometrySets = {};

    // for offsetof only
    struct VertexData {
        glm::vec4 vertex = glm::vec4(0.f, 0.f, 0.f, 1.f);
        glm::vec4 texcoord = glm::vec4(0.f, 0.f, 0.f, 0.f);
        glm::vec4 normal = glm::vec4(0.f, 1.f, 0.f, 0.f);
    };

    // per every geometry
    const uint32_t accessorCount = 3;
    const uint32_t bindingsCount = 1;
};

// implementation for C API (bridge between C++ and C)
extern "C" {
    #include "./renderer.h"

    // initialize defaults for C
#ifdef VLR_C_RENDERER
    void initMaterialUnit(CMaterialUnit* unit) {
        auto material = vlr::MaterialUnit{};
        *unit = reinterpret_cast<const CMaterialUnit&>(material);
    };
#endif
};

// implementation for C++ wrapper API (bridge between hardcode and softcode)
#include "./renderer.hpp"
namespace vrp {
#ifdef VLR_CPP_RENDERER

#endif
};
