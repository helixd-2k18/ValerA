#pragma once

#include <vlr/Implementation.hpp>
#include <vlr/Renderer.hpp>

namespace vlr {
    void Renderer::initFramebuffer(const uint32_t& width, const uint32_t& height){
        framebuffer.createFramebuffer(width, height);
        layout->setFramebuffer(framebuffer);
    };

    void Renderer::constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<RendererCreateInfo> info){
        // 
        constants = std::make_shared<vlr::Constants>(driver, vlr::DataSetCreateInfo{ .count = 1u, .uniform = true });
        layout = std::make_shared<vlr::PipelineLayout>(driver);
        materialSet = std::make_shared<vlr::MaterialSet>(driver, vlr::DataSetCreateInfo{ .count = 256 });
        textureSet = std::make_shared<vlr::TextureSet>(driver);
        samplerSet = std::make_shared<vlr::SamplerSet>(driver);
        background = std::make_shared<vlr::Background>(driver);

        // 
        bindingSet = std::make_shared<vlr::BindingSet>(driver, vlr::DataSetCreateInfo{ .count = info->maxGeometries });
        attributeSet = std::make_shared<vlr::AttributeSet>(driver, vlr::DataSetCreateInfo{ .count = info->maxGeometries * info->attributePerGeometry });
        bufferViews = std::make_shared<vlr::BufferViewSet>(driver);
        vertexSet = std::make_shared<vlr::VertexSet>(driver, vlr::VertexSetCreateInfo{
            .bindings = bindingSet,
            .attributes = attributeSet,
            .bufferViews = bufferViews
        });

        // 
        rasterization = std::make_shared<vlr::Rasterization>(driver, vlr::PipelineCreateInfo{
            .layout = layout,
            .framebuffer = framebuffer,
            .instanceSet = instanceSet,
            .constants = constants
        });

        // 
        rayTracing = std::make_shared<vlr::RayTracing>(driver, vlr::RayTracingCreateInfo{
            .layout = layout,
            .framebuffer = framebuffer,
            .accelerationTop = accelerationTop,
            .constants = constants
        });

        // make instanceSet
        instanceSet = std::make_shared<vlr::InstanceSet>(driver, vlr::DataSetCreateInfo{ .count = info->maxInstances });
        accelerationTop = std::make_shared<vlr::Acceleration>(driver, vlr::AccelerationCreateInfo{ .instanceSet = instanceSet, .initials = { info->maxInstances } }); // shapes.size()

        // 
        framebuffer = std::make_shared<vlr::Framebuffer>(driver);
        framebuffer.createRenderPass();

        // 
        layout->setBackground(background);
        layout->setMaterials(materialSet, textureSet, samplerSet);
        layout->setVertexSet(vertexSet);

        //
        renderCommand = std::make_shared<vlr::RenderCommand>(driver, vlr::RenderCommandCreateInfo{
            .layout = layout,
            .rayTracing = rayTracing,
            .rasterization = rasterization
        });

        // 
        buildCommand = std::make_shared<vlr::BuildCommand>(driver, vlr::BuildCommandCreateInfo{
            .layout = layout,
            .accelerationTop = accelerationTop,
        });

        // 
        rasterization->setDescriptorSets(layout);
        rayTracing->setDescriptorSets(layout);
        renderCommand->setDescriptorSets(layout);
        buildCommand->setDescriptorSets(layout);
    }
};
