#pragma once

// 
#include <vlr/Config.hpp>

// Planned C++20 version of wrapper (faster C++ compilation time)
#ifndef VLR_CPP_RENDERER
#define VLR_CPP_RENDERER

//namespace vrc {
//
//};

// 
namespace vrp {

// type-safe identifiers
#define TYPED(NAME) \
class NAME {public: \
int32_t ID = 0u;\
NAME(const int32_t& id = 0u) { ID = id; };\
inline operator int32_t& () {return ID;};\
inline operator const int32_t& () const {return ID;};\
inline NAME& operator =(const int32_t& id) {this->ID = id; return *this;};\
};

    // 
    TYPED(Base);

    //
    class Image2D : public Base {
        Image2D(const int32_t& id) : Base(id) {};
        Image2D(const uint32_t& width = 2u, const uint32_t& height = 2, const VkFormat& format = VK_FORMAT_R8G8B8A8_UNORM, const uint32_t& levels = 1u);
    };

    //
    class BufferSet : public Base {
        BufferSet(const int32_t& id) : Base(id) {};
        BufferSet(const VkDeviceSize& count = 1u, const bool& uniform = false);

        // 
        void copyFromCpu();
        void copyToImage(const Image2D& image);
        void* map();
        const void* map() const;
    };

    //
    class Geometry : public Base {
        Geometry(const int32_t& id) : Base(id) {};
        Geometry(const BufferSet& vertexData, const BufferSet& indexData, const vlr::GeometryDesc& desc);

        // 
        
    };

    //
    class GeometrySet : public Base {
        GeometrySet(const int32_t& id) : Base(id) {};
        GeometrySet(const std::vector<Geometry>& geoms = {});

        // 
        void setCommand(const VkCommandBuffer& cmd);
    };


    //
    void instanceCommand(const VkCommandBuffer& cmd);
    void rayTraceCommand(const VkCommandBuffer& cmd);
    void geometryCommand(const VkCommandBuffer& cmd);
    void drawObject(const GeometrySet& set);

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
    void initVertexLayout(const uint32_t& stride, const Index& indexType, const LayoutPreset& preset);
    void initFramebuffer(const uint32_t& width = 2u, const uint32_t& height = 2u);
    void initialize(const uint32_t& deviceID = 0u);
    void* getDriver();
};

#endif
