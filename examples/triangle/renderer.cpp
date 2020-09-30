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

    // geometries
    inline static vlj::VertexSet vertexSet = {};
    inline static vlj::BufferViewSet bufferViews = {};
    inline static vlj::AttributeSet attributeSet = {};
    inline static vlj::BindingSet bindingSet = {};
    inline static std::vector<vlj::Geometry> geometries = {};

    // bodies
    inline static std::vector<vlj::GeometrySet> geometrySets = {};
    inline static std::vector<vlj::Acceleration> accelerations = {};

    // draw calls
    inline static vlj::InstanceSet instanceSet = {};
    inline static vlj::Acceleration accelerationTop = {};

    // materials
    inline static vlj::MaterialSet materialSet = {};
    inline static vlj::TextureSet textureSet = {};
    inline static vlj::SamplerSet samplerSet = {};

    // low-level data
    inline static std::vector<vkt::ImageRegion> images = {};
    inline static std::vector<vlj::SetBase> buffers = {};

    // 
    class Slots { public: 
        uint32_t count = 0u;
        std::vector<int32_t> available = {};

        // 
        Slots() {};
        Slots(const Slots& slots) : count(slots.count), available(slots.available) {};
        Slots(const uint32_t& maxCount) {
            available.clear();
            available.resize(0u);
            for (uint32_t i = 0; i < maxCount; i++) {
                available.push_back(i);
            };
        };

        // 
        Slots& operator=(const Slots& slots) {
            this->count = slots.count, this->available = slots.available;
        };

        // 
        int32_t consume() {
            int32_t id = available[count];
            if (count < available.size()) {
                int32_t& idr = available[count++]; idr = -1;
            };
            return id;
        };

        // 
        void rise(const int32_t& used) {
            if (count > 0 && used != -1 && available[count-1] == -1) {
                available[--count] = used; 
            };
        };
    };

    // data slots
    inline static Slots availableTextures = {};
    inline static Slots availableSamplers = {};
    inline static Slots availableMaterials = {};

    // geometry slots
    inline static Slots availableGeometries = {};
    inline static Slots availableGeometrySets = {};

    // for offsetof only, NOT external usage
    struct VertexData {
        glm::vec4 vertex = glm::vec4(0.f, 0.f, 0.f, 1.f);
        glm::vec4 normal = glm::vec4(0.f, 1.f, 0.f, 0.f);
        glm::vec4 texcoord = glm::vec4(0.f, 0.f, 0.f, 0.f);
    };

    // per every geometry
    const uint32_t accessorCount = 3, bindingsCount = 1;

    // 
    void initialize(const uint32_t& deviceID) {

    };
};

// implementation for C API (bridge between C++ and C)
extern "C" {
    #include "./renderer.h"

    // initialize defaults for C
#ifdef VLR_C_RENDERER
    void initMaterialUnit(CMaterialUnit* cunit) {
        const auto unit = vlr::MaterialUnit{};
        *cunit = reinterpret_cast<const CMaterialUnit&>(unit);
    };

    void initVertexData(CVertexData* cunit) {
        const auto unit = vrc::VertexData{};
        *cunit = reinterpret_cast<const CVertexData&>(unit);
    };
#endif
};

// implementation for C++ wrapper API (bridge between hardcode and softcode)
#include "./renderer.hpp"
namespace vrp {
#ifdef VLR_CPP_RENDERER

#endif
};
