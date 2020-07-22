#include "./vlr/RayTracing.hpp"
#include "./vlr/Framebuffer.hpp"
#include "./vlr/SetBase.hpp"
#include "./vlr/BufferViewSet.hpp"

// 
namespace vlr {

    void RayTracing::constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<PipelineCreateInfo> info) {
        this->driver = driver, this->layout = info->layout, this->framebuffer = info->framebuffer; auto device = this->driver->getDeviceDispatch();

        this->stages = { // for faster code, pre-initialize
            vkt::makePipelineStageInfo(device, vkt::readBinary(std::string("./shaders/generation.comp.spv")), VK_SHADER_STAGE_COMPUTE_BIT),
            vkt::makePipelineStageInfo(device, vkt::readBinary(std::string("./shaders/intersection.comp.spv")), VK_SHADER_STAGE_COMPUTE_BIT),
            vkt::makePipelineStageInfo(device, vkt::readBinary(std::string("./shaders/interpolation.comp.spv")), VK_SHADER_STAGE_COMPUTE_BIT),
            vkt::makePipelineStageInfo(device, vkt::readBinary(std::string("./shaders/finalize.comp.spv")), VK_SHADER_STAGE_COMPUTE_BIT)
        };

        // 
        this->generation = vkt::createCompute(this->driver->getDeviceDispatch(), vkt::FixConstruction(this->stages[0]), layout->pipelineLayout, this->driver->getPipelineCache());
        this->interpolation = vkt::createCompute(this->driver->getDeviceDispatch(), vkt::FixConstruction(this->stages[0]), layout->pipelineLayout, this->driver->getPipelineCache());
        this->intersection = vkt::createCompute(this->driver->getDeviceDispatch(), vkt::FixConstruction(this->stages[0]), layout->pipelineLayout, this->driver->getPipelineCache());
        this->finalize = vkt::createCompute(this->driver->getDeviceDispatch(), vkt::FixConstruction(this->stages[0]), layout->pipelineLayout, this->driver->getPipelineCache());

        // 
        this->rayDataFlip0 = std::make_shared<SetBase_T<RayData>>(driver, DataSetCreateInfo{ .count = 4096 * 4096 });
        this->rayDataFlip1 = std::make_shared<SetBase_T<RayData>>(driver, DataSetCreateInfo{ .count = 4096 * 4096 });
        this->counters = std::make_shared<SetBase_T<uint32_t>>(driver, DataSetCreateInfo{ .count = 4 }); // Ray Write, Ray Read, Hit Write, Hit Read counters

        // 
        this->rayDataSetFlip0 = std::make_shared<BufferViewSet>(driver);
        this->rayDataSetFlip1 = std::make_shared<BufferViewSet>(driver);

        // Flip-able ray data
        this->rayDataSetFlip0->pushBufferView(this->rayDataFlip0->getGpuBuffer()); // Write-able
        this->rayDataSetFlip0->pushBufferView(this->rayDataFlip1->getGpuBuffer()); // Read-only 
        this->rayDataSetFlip1->pushBufferView(this->rayDataFlip1->getGpuBuffer()); // Write-able
        this->rayDataSetFlip1->pushBufferView(this->rayDataFlip0->getGpuBuffer()); // Read-only 

        // 
        this->rayDataSetFlip0->createDescriptorSet(layout);
        this->rayDataSetFlip1->createDescriptorSet(layout);
        this->counters->createDescriptorSet(layout);

        // 
        this->layout->bound[12u] = this->rayDataSetFlip0->set;
    };

    void RayTracing::setCommand(vkt::uni_arg<VkCommandBuffer> currentCmd, const glm::uvec4& vect0) {
        const auto& viewport = reinterpret_cast<vkh::VkViewport&>(framebuffer->viewport);
        const auto& renderArea = reinterpret_cast<vkh::VkRect2D&>(framebuffer->scissor);
        auto device = this->driver->getDeviceDispatch();
        glm::uvec4 meta = vect0;

        {   // Denoise diffuse data
            device->CmdBindPipeline(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, this->generation);
            device->CmdBindDescriptorSets(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, layout->pipelineLayout, 0u, layout->bound.size(), layout->bound.data(), 0u, nullptr);
            device->CmdPushConstants(currentCmd, layout->pipelineLayout, layout->stages, 0u, sizeof(glm::uvec4), &meta);
            device->CmdDispatch(currentCmd, vkt::tiled(renderArea.extent.width, 32u), vkt::tiled(renderArea.extent.height, 12u), 1u);
            vkt::commandBarrier(device, currentCmd);
            //this->swapRayData();
        };

        // 
        for (uint32_t i=0;i<3;i++) {
            {   // Intersect All
                device->CmdBindPipeline(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, this->intersection);
                device->CmdBindDescriptorSets(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, layout->pipelineLayout, 0u, layout->bound.size(), layout->bound.data(), 0u, nullptr);
                device->CmdPushConstants(currentCmd, layout->pipelineLayout, layout->stages, 0u, sizeof(glm::uvec4), &meta);
                device->CmdDispatch(currentCmd, 256u, 1u, 1u);
                vkt::commandBarrier(device, currentCmd);
                this->swapRayData(); // After intersection, needs prepare for newer rays
            };

            {   // 
                const uint32_t zero = 0u;
                std::vector<vkh::VkBufferCopy> regions = {
                    { .srcOffset = 0ull                   , .dstOffset = 1ull * sizeof(uint32_t), .size = sizeof(uint32_t) },
                    { .srcOffset = 2ull * sizeof(uint32_t), .dstOffset = 3ull * sizeof(uint32_t), .size = sizeof(uint32_t) }
                };
                device->CmdCopyBuffer(currentCmd, this->counters->getGpuBuffer(), this->counters->getGpuBuffer(), regions.size(), reinterpret_cast<const VkBufferCopy*>(regions.data()));
                vkt::commandBarrier(device, currentCmd); // TODO: Advanced Barrier
                device->CmdUpdateBuffer(currentCmd, this->counters->getGpuBuffer(), 0ull                   , sizeof(uint32_t), &zero);
                device->CmdUpdateBuffer(currentCmd, this->counters->getGpuBuffer(), 2ull * sizeof(uint32_t), sizeof(uint32_t), &zero);
                vkt::commandBarrier(device, currentCmd); // TODO: Advanced Barrier
            };

            {   // Interpolate and Re-Generate Rays
                device->CmdBindPipeline(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, this->interpolation);
                device->CmdBindDescriptorSets(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, layout->pipelineLayout, 0u, layout->bound.size(), layout->bound.data(), 0u, nullptr);
                device->CmdPushConstants(currentCmd, layout->pipelineLayout, layout->stages, 0u, sizeof(glm::uvec4), &meta);
                device->CmdDispatch(currentCmd, 256u, 1u, 1u);
                vkt::commandBarrier(device, currentCmd);
            };
        };

        {   // Finalize Ray Tracing
            device->CmdBindPipeline(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, this->finalize);
            device->CmdBindDescriptorSets(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, layout->pipelineLayout, 0u, layout->bound.size(), layout->bound.data(), 0u, nullptr);
            device->CmdPushConstants(currentCmd, layout->pipelineLayout, layout->stages, 0u, sizeof(glm::uvec4), &meta);
            device->CmdDispatch(currentCmd, 256u, 1u, 1u);
            vkt::commandBarrier(device, currentCmd);
        };
    };

};
