#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
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

        // 
        VkPipeline pipeline = VK_NULL_HANDLE, opaque = VK_NULL_HANDLE;
        //uint32_t geometryID = 0u; // 
        //uint32_t instanceID = 0u; // 

        // 
        vkt::uni_ptr<BufferViewSet> geometriesDescs = {}; // Buffers with GeometryDesc

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

        // Set Constants Buffer
        virtual void setConstants(vkt::uni_ptr<Constants> constants) {
            this->constants = constants;
        };
    };

};

namespace vlj {
    class Rasterization : public Wrap<vlr::Rasterization> {
    public:
        Rasterization() : Wrap<vlr::Rasterization>() {};
        Rasterization(vkt::uni_ptr<vlr::Rasterization> object) : Wrap<vlr::Rasterization>(object) {};
        Rasterization(vkt::uni_ptr<vlr::Driver> driver, vkt::uni_arg<vlr::PipelineCreateInfo> info) : Wrap<vlr::Rasterization>(std::make_shared<vlr::Rasterization>(driver, info)) {};
        Rasterization(std::shared_ptr<vlr::Rasterization> object) : Wrap<vlr::Rasterization>(object) {};

        //CALLIFY(constructor);
        CALLIFY(drawCommand);
        CALLIFY(setCommand);
        CALLIFY(setDescriptorSets);
        CALLIFY(setInstanceSet);
        CALLIFY(setConstants);
    };
};
