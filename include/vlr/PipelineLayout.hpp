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
        vkt::uni_ptr<Driver> driver = {};
        vkh::VkShaderStageFlags stages = {};

    public: 
        PipelineLayout() { this->constructor(); };
        PipelineLayout(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };
        ~PipelineLayout() {};

        // 
        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver);

        // 
        virtual VkPipelineCache getPipelineCache() const { return driver->getPipelineCache(); };
        virtual VkPipelineLayout getBindingPipelineLayout() const { return pipelineLayout; };
        virtual VkPipelineLayout getTransformPipelineLayout() const { return transformLayout; };
        virtual VkDescriptorPool getDescriptorPool() const { return driver->getDescriptorPool(); };

        virtual VkDescriptorSetLayout getDescriptorSetLayout(const uint32_t& I) const { return layouts[I]; };
        virtual VkDescriptorSetLayout getSetLayout() const { return layouts[0u]; };
        virtual VkDescriptorSetLayout getTextureSetLayout() const { return layouts[1u]; };
        virtual VkDescriptorSetLayout getSamplerSetLayout() const { return layouts[2u]; };
        virtual VkDescriptorSetLayout getUniformSetLayout() const { return layouts[4u]; };
        virtual VkDescriptorSetLayout getBufferViewSetLayout() const { return layouts[3u]; };
        virtual VkDescriptorSetLayout getBackgroundSetLayout() const { return layouts[5u]; };
        virtual VkDescriptorSetLayout getFramebufferSetLayout() const { return layouts[6u]; };
        virtual VkDescriptorSetLayout getAccelerationSetLayout() const { return layouts[7u]; };

        // 
        virtual void setMaterials(vkt::uni_ptr<MaterialSet> materialSet, vkt::uni_ptr<TextureSet> textureSet, vkt::uni_ptr<SamplerSet> samplerSet);
        virtual void setVertexData(vkt::uni_ptr<VertexSet> vertexData);
        virtual void setFramebuffer(vkt::uni_ptr<Framebuffer> framebuffer);
        virtual void setAccelerationTop(vkt::uni_ptr<Acceleration> acceleration);
        virtual void setConstants(vkt::uni_ptr<Constants> constants);
        virtual void setRayTracing(vkt::uni_ptr<RayTracing> rayTracing);
        virtual void setRasterization(vkt::uni_ptr<Rasterization> rayTracing);
        virtual void setInstanceSet(vkt::uni_ptr<InstanceSet> instanceSet);
        virtual void setBackground(vkt::uni_ptr<Background> background);

        // 
        virtual VkPipelineLayout& getPipelineLayout() { return this->pipelineLayout; };
        virtual const VkPipelineLayout& getPipelineLayout() const { return this->pipelineLayout; };

        // 
        virtual VkDescriptorSet& getDescriptorSet(const uint32_t& I) { return this->bound[I]; };
        virtual const VkDescriptorSet& getDescriptorSet(const uint32_t& I) const { return this->bound[I]; };

        // 
        virtual std::vector<VkDescriptorSet>& getDescriptorSets() { return this->bound; };
        virtual const std::vector<VkDescriptorSet>& getDescriptorSets() const { return this->bound; };
    };

};
