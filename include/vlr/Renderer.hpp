#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

// 
#include <vlr/Constants.hpp>
#include <vlr/BufferViewSet.hpp>
#include <vlr/MaterialSet.hpp>
#include <vlr/TextureSet.hpp>
#include <vlr/SamplerSet.hpp>
#include <vlr/Background.hpp>

// 
#include <vlr/GeometrySet.hpp>
#include <vlr/Acceleration.hpp>
#include <vlr/BuildCommand.hpp>

// 
#include <vlr/RayTracing.hpp>
#include <vlr/Rasterization.hpp>
#include <vlr/RenderCommand.hpp>

namespace vlr {
    
    struct RendererCreateInfo {
        uint32_t maxGeometries = 256u;
        uint32_t maxInstances = 256u;
        uint32_t attributePerGeometry = 4u;
    };

    const uint32_t FBufID = 4u;

    class Renderer : public std::enable_shared_from_this<Renderer> { public: 
        Renderer() { this->constructor(); };
        Renderer(vkt::uni_ptr<Driver> driver, vkt::uni_arg<RendererCreateInfo> info = RendererCreateInfo{}) { this->constructor(driver, info); };

    protected:
        virtual void constructor(){};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<RendererCreateInfo> info = RendererCreateInfo{});

    public:
        virtual vlj::Geometry createGeometry(vkt::uni_arg<GeometryDesc> desc = {}) {
            return std::make_shared<vlr::Geometry>(vertexSet, desc);
        };
        virtual vlj::GeometrySet createGeometrySet(VkDeviceSize count = 1u) {
            return std::make_shared<vlr::GeometrySet>(vertexSet, vlr::DataSetCreateInfo{ .count = count });
        };

        virtual void initFramebuffer(const uint32_t& width, const uint32_t& height);
        
        virtual void instanceCommand(const VkCommandBuffer& cmd){
            instanceSet->setCommand(cmd, true);
            buildCommand->setCommandTop(cmd);
            instanceCount = 0u;
        };

        virtual void rayTraceCommand(const VkCommandBuffer& cmd){
            materialSet->setCommand(cmd);
            constants->setCommand(cmd, true);
            renderCommand->setCommand(cmd);
            framebuffer->imageToLinearCopyCommand(cmd, FBufID);
            rayTracing->setCommandFinal(cmd);
        };

        virtual void geometryCommand(const VkCommandBuffer& cmd){
            bindingSet->setCommand(cmd);
            attributeSet->setCommand(cmd);
        };

        virtual intptr_t addBufferView(vkt::VectorBase buffer){
            return bufferViews->pushBufferView(buffer);
        };

        virtual intptr_t addAcceleration(vlj::Acceleration acceleration){
            intptr_t ptr = accelerations.size();
            accelerations.push_back(acceleration);
            geometrySets.push_back(acceleration.getGeometrySet());
            return ptr;
        };

        virtual void createInstance(vkh::VsGeometryInstance instanceDesc){
            instanceDesc.accelerationStructureHandle = accelerations[instanceDesc.customId]->getHandle();
            instanceSet->get(instanceCount++) = instanceDesc;
        };

        virtual void setBackground(vkt::ImageRegion image) {
            background->setImage(image);
        };

        virtual intptr_t pushTexture(vkt::ImageRegion image) {
            return textureSet->pushImage(image);
        };

        virtual intptr_t pushSampler(VkSampler sampler) {
            return samplerSet->pushSampler(sampler);
        };

        virtual vlr::MaterialUnit& editMaterial(intptr_t ptr = 0u) {
            return *materialSet.get(ptr);
        };

        virtual VkVertexInputAttributeDescription& editAttribute(intptr_t ptr = 0u) {
            return *attributeSet.get(ptr);
        };

        virtual VkVertexInputBindingDescription& editBinding(intptr_t ptr = 0u) {
            return *bindingSet.get(ptr);
        };

        virtual vlr::ConstantDesc& editConstants() {
            return *constants.get(0u);
        };

        virtual VkPipelineLayout& getPipelineLayout() {
            return layout->getPipelineLayout();
        };

        virtual std::vector<VkDescriptorSet>& getDescriptorSets() {
            return layout->getDescriptorSets();
        };

    protected: 
        inline static vlj::Driver driver = {};
        inline static uint32_t instanceCount = 0u;
        inline static uint32_t materialCount = 0u;

        // geometries
        inline static vlj::Framebuffer framebuffer = {};
        inline static vlj::PipelineLayout layout = {};
        inline static vlj::Background background = {};
        inline static vlj::Constants constants = {};
        inline static vlj::Rasterization rasterization = {};
        inline static vlj::RayTracing rayTracing = {};

        //
        inline static vlj::VertexSet vertexSet = {};
        inline static vlj::BufferViewSet bufferViews = {};
        inline static vlj::AttributeSet attributeSet = {};
        inline static vlj::BindingSet bindingSet = {};
        //inline static std::vector<vlj::Geometry> geometries = {};

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

        //
        inline static vlj::RenderCommand renderCommand = {};
        inline static vlj::BuildCommand buildCommand = {};
    };

};
