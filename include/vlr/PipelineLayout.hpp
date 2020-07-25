#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

// 
#include "./BufferViewSet.hpp"
#include "./MaterialSet.hpp"
#include "./TextureSet.hpp"
#include "./SamplerSet.hpp"

//
#include "./VertexSet.hpp"
#include "./Constants.hpp"

// 
namespace vlr {

    class PipelineLayout : public std::enable_shared_from_this<PipelineLayout> { protected: friend Rasterization; friend Resampling; friend RayTracing;
        VkPipelineCache cache = {};
        VkPipelineLayout pipelineLayout = {}, transformLayout = {}; // Unified Bindings, Transform Feedback 
        //VkDescriptorPool pool = {};
        std::vector<VkDescriptorSet> bound = {};
        
        // 
        std::vector<VkDescriptorSetLayout> layouts = {};
        std::shared_ptr<Driver> driver = {};
        vkh::VkShaderStageFlags stages = {};
        
    public: 
        PipelineLayout() { this->constructor(); };
        PipelineLayout(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        // 
        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver);

        // 
        VkPipelineCache getPipelineCache() const { return driver->getPipelineCache(); };
        VkPipelineLayout getBindingPipelineLayout() const { return pipelineLayout; };
        VkPipelineLayout getTransformPipelineLayout() const { return transformLayout; };
        VkDescriptorPool getDescriptorPool() const { return driver->getDescriptorPool(); };

        VkDescriptorSetLayout getDescriptorSetLayout(const uint32_t& I) const { return layouts[I]; };
        VkDescriptorSetLayout getSetLayout() const { return layouts[0u]; };
        VkDescriptorSetLayout getTextureSetLayout() const { return layouts[1u]; };
        VkDescriptorSetLayout getSamplerSetLayout() const { return layouts[2u]; };
        VkDescriptorSetLayout getUniformSetLayout() const { return layouts[4u]; };
        VkDescriptorSetLayout getBufferViewSetLayout() const { return layouts[3u]; };
        VkDescriptorSetLayout getBackgroundSetLayout() const { return layouts[5u]; };
        VkDescriptorSetLayout getFramebufferSetLayout() const { return layouts[6u]; };
        VkDescriptorSetLayout getAccelerationSetLayout() const { return layouts[7u]; };

        // 
        virtual void setMaterials(vkt::uni_ptr<MaterialSet> materialSet, vkt::uni_ptr<TextureSet> textureSet, vkt::uni_ptr<SamplerSet> samplerSet);
        virtual void setVertexData(vkt::uni_ptr<VertexSet> vertexData);
        virtual void setFramebuffer(vkt::uni_ptr<Framebuffer> framebuffer);

        // 
        virtual void setAccelerationTop(vkt::uni_ptr<Acceleration> acceleration);
        virtual void setConstants(vkt::uni_ptr<Constants> constants);
        virtual void setRayTracing(vkt::uni_ptr<RayTracing> rayTracing);
    };



};
