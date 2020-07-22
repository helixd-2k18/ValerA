#pragma once
#include "./Config.hpp"
#include "./Framebuffer.hpp"

namespace vlr {

    class RayTracing : public std::enable_shared_from_this<RayTracing> { protected: 
        vkt::uni_ptr<Acceleration> accelerationTop = {}; // Top Level Acceleration Structure
        vkt::uni_ptr<VertexSet> vertexSet = {}; // Vertex Set 
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<TextureSet> textureSet = {};
        vkt::uni_ptr<SamplerSet> samplerSet = {};
        vkt::uni_ptr<MaterialSet> materialSet = {};
        vkt::uni_ptr<Framebuffer> framebuffer = {};
        vkt::uni_ptr<Driver> driver = {};

        //std::vector<vkt::uni_ptr<GeometrySet>> geometries = {}; // Not Necessary, inbound with `vkt::uni_ptr<Acceleration>`
        std::vector<vkt::uni_ptr<Acceleration>> accelerations = {};
        VkPipeline generation = VK_NULL_HANDLE, interpolation = VK_NULL_HANDLE, intersection = VK_NULL_HANDLE;

    public: 
        RayTracing() { this->constructor(); };
        RayTracing(vkt::uni_ptr<Driver> driver, vkt::uni_arg<PipelineCreateInfo> info = {}) { this->constructor(driver, info); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<PipelineCreateInfo> info = {});
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> rasterCommand, const glm::uvec4& meta = glm::uvec4(0u));
    };

};
