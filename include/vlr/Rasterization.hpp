#pragma once
#include "./Config.hpp"
#include "./PipelineLayout.hpp"
#include "./Framebuffer.hpp"
#include "./Geometry.hpp"
#include "./Constants.hpp"

namespace vlr {

    class Rasterization : public std::enable_shared_from_this<Rasterization> { protected: friend RayTracing; // 
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<TextureSet> textureSet = {};
        vkt::uni_ptr<SamplerSet> samplerSet = {};
        vkt::uni_ptr<MaterialSet> materialSet = {};
        vkt::uni_ptr<Framebuffer> framebuffer = {};
        vkt::uni_ptr<Constants> constants = {};
        vkt::uni_ptr<Driver> driver = {};

        // 
        //vkt::uni_ptr<GeometrySet> geometrySet = {};
        //vkt::uni_ptr<InstanceSet> instanceSet = {};
        vkt::uni_ptr<InstanceSet> instanceSet = {}; // Used By Top Level
        std::vector<vkt::uni_ptr<GeometrySet>> geometrySets = {}; // Not Necessary, inbound with `vkt::uni_ptr<Acceleration>`, Used By Bottom Levels

        // 
        VkPipeline pipeline = VK_NULL_HANDLE;
        //uint32_t geometryID = 0u; // 
        //uint32_t instanceID = 0u; // 

        // 
        vkt::uni_ptr<BufferViewSet> geometriesDescs = {}; // Buffers with GeometryDesc
        vkt::uni_ptr<BufferViewSet> interpolations = {};

        // 
        std::vector<vkh::VkPipelineShaderStageCreateInfo> stages = {};
        vkh::VsGraphicsPipelineCreateInfoConstruction pipelineInfo = {};
        VkPipelineRasterizationConservativeStateCreateInfoEXT conserv = {};

    public: 
        Rasterization() { this->constructor(); };
        Rasterization(vkt::uni_ptr<Driver> driver, vkt::uni_arg<PipelineCreateInfo> info) { this->constructor(driver, info); };

        // 
        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<PipelineCreateInfo> info);
        virtual void drawCommand(vkt::uni_arg<VkCommandBuffer> rasterCommand, const glm::uvec4& meta = glm::uvec4(0u));
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> rasterCommand, const glm::uvec4& meta = glm::uvec4(0u));
        virtual void setDescriptorSets(vkt::uni_ptr<PipelineLayout> ilayout = {});

        // 
        virtual void setInstanceSet(vkt::uni_ptr<InstanceSet> instanceSet) {
            this->instanceSet = instanceSet;
        };

        // 
        virtual void setGeometrySets(std::vector<vkt::uni_ptr<GeometrySet>> geometrySets) {
            this->geometrySets = geometrySets;
        };

        // 
        virtual void pushGeometrySet(vkt::uni_ptr<GeometrySet> geometrySet) {
            this->geometrySets.push_back(geometrySet);
        };

        // 
        virtual void resetGeometrySets(vkt::uni_ptr<GeometrySet> geometrySet) {
            this->geometrySets.resize(0u);
        };

        // Set Constants Buffer
        virtual void setConstants(vkt::uni_ptr<Constants> constants) {
            this->constants = constants;
        };
    };

};
