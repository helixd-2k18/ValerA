#include "./vlr/RayTracing.hpp"
#include "./vlr/Framebuffer.hpp"
#include "./vlr/SetBase.hpp"
#include "./vlr/Acceleration.hpp"
#include "./vlr/BufferViewSet.hpp"
#include "./vlr/Interpolation.hpp"
#include "./vlr/Constants.hpp"

// 
namespace vlr {

    void RayTracing::constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<RayTracingCreateInfo> info) {
        if (info.has()) {
            this->layout = info->layout, this->framebuffer = info->framebuffer, this->accelerationTop = info->accelerationTop, this->accelerations = info->accelerations, this->constants = info->constants; 
        };
        this->driver = driver;
        auto device = this->driver->getDeviceDispatch();

        // 
        this->stages = { // for faster code, pre-initialize
            vkt::makePipelineStageInfo(device, vkt::readBinary(std::string("./shaders/generation.comp.spv")), VK_SHADER_STAGE_COMPUTE_BIT),
            vkt::makePipelineStageInfo(device, vkt::readBinary(std::string("./shaders/intersection.comp.spv")), VK_SHADER_STAGE_COMPUTE_BIT),
            vkt::makePipelineStageInfo(device, vkt::readBinary(std::string("./shaders/interpolation.comp.spv")), VK_SHADER_STAGE_COMPUTE_BIT),
            vkt::makePipelineStageInfo(device, vkt::readBinary(std::string("./shaders/finalize.comp.spv")), VK_SHADER_STAGE_COMPUTE_BIT),
            vkt::makePipelineStageInfo(device, vkt::readBinary(std::string("./shaders/resample.comp.spv")), VK_SHADER_STAGE_COMPUTE_BIT)
        };

        // 
        this->generation = vkt::createCompute(this->driver->getDeviceDispatch(), vkt::FixConstruction(this->stages[0]), layout->pipelineLayout, this->driver->getPipelineCache());
        this->interpolation = vkt::createCompute(this->driver->getDeviceDispatch(), vkt::FixConstruction(this->stages[1]), layout->pipelineLayout, this->driver->getPipelineCache());
        this->intersection = vkt::createCompute(this->driver->getDeviceDispatch(), vkt::FixConstruction(this->stages[2]), layout->pipelineLayout, this->driver->getPipelineCache());
        this->finalize = vkt::createCompute(this->driver->getDeviceDispatch(), vkt::FixConstruction(this->stages[3]), layout->pipelineLayout, this->driver->getPipelineCache());
        this->resample = vkt::createCompute(this->driver->getDeviceDispatch(), vkt::FixConstruction(this->stages[4]), layout->pipelineLayout, this->driver->getPipelineCache());

        // 
        this->rayDataFlip0 = std::make_shared<SetBase_T<RayData>>(driver, DataSetCreateInfo{ .count = 4096 * 4096, .enableCPU = false });
        this->rayDataFlip1 = std::make_shared<SetBase_T<RayData>>(driver, DataSetCreateInfo{ .count = 4096 * 4096, .enableCPU = false });
        this->hitData  = std::make_shared<SetBase_T<HitData>>(driver, DataSetCreateInfo{ .count = 4096 * 4096, .enableCPU = false });
        this->colorChainData = std::make_shared<SetBase_T<ColorData>>(driver, DataSetCreateInfo{ .count = 4096 * 4096, .enableCPU = false });
        this->counters = std::make_shared<SetBase_T<uint32_t>>(driver, DataSetCreateInfo{ .count = 5 }); // Ray Write, Ray Read, Hit Write, Hit Read, Color Chain counters

        // 
        this->rayDataSetFlip0 = std::make_shared<BufferViewSet>(driver);
        this->rayDataSetFlip1 = std::make_shared<BufferViewSet>(driver);

        // Flip-able ray data
        this->rayDataSetFlip0->pushBufferView(this->rayDataFlip0->getGpuBuffer()); // Write-able
        this->rayDataSetFlip0->pushBufferView(this->rayDataFlip1->getGpuBuffer()); // Read-only 
        this->rayDataSetFlip1->pushBufferView(this->rayDataFlip1->getGpuBuffer()); // Write-able
        this->rayDataSetFlip1->pushBufferView(this->rayDataFlip0->getGpuBuffer()); // Read-only 

        // Re-Create
        this->interpolations = std::make_shared<BufferViewSet>(this->driver);
        this->geometriesDescs = std::make_shared<BufferViewSet>(this->driver);
    };

    void RayTracing::setDescriptorSets(vkt::uni_ptr<PipelineLayout> ilayout) { // 
        if (layout.has()) { this->layout = ilayout; };

        this->rayDataSetFlip0->createDescriptorSet(layout);
        this->rayDataSetFlip1->createDescriptorSet(layout);
        this->colorChainData->createDescriptorSet(layout);
        this->hitData->createDescriptorSet(layout);
        this->counters->createDescriptorSet(layout);

        // Check if exist...
        if (this->accelerationTop.has() && this->accelerations.size() > 0 && this->accelerationTop->instanceSet.has()) { // 
            // 
            this->interpolations->resetBufferViews();
            this->geometriesDescs->resetBufferViews();

            // 
            for (uint32_t i=0;i<accelerations.size();i++) {
                vkt::uni_ptr<GeometrySet> geometrySet = accelerations[i]->geometrySet;
                vkt::uni_ptr<Interpolation> interpolation = geometrySet->interpolations;

                // 
                this->interpolations->pushBufferView(interpolation->getGpuBuffer());
                this->geometriesDescs->pushBufferView(geometrySet->getGpuBuffer());
            };

            // 
            this->interpolations->createDescriptorSet(layout);
            this->geometriesDescs->createDescriptorSet(layout);
        };

        // TODO: Decise by PipelineLayout class
        this->layout->bound[8u] = this->geometriesDescs->set;
        this->layout->bound[9u] = this->interpolations->set;

        // 
        this->layout->setAccelerationTop(this->accelerationTop);
        this->layout->setConstants(this->constants);

        // 
        this->layout->bound[11u] = this->counters->set;
        this->layout->bound[12u] = this->rayDataSetFlip0->set;
        this->layout->bound[13u] = this->hitData->set;
        this->layout->bound[14u] = this->colorChainData->set;
        
    };

    void RayTracing::setCommand(vkt::uni_arg<VkCommandBuffer> currentCmd, const glm::uvec4& vect0) {
        const auto& viewport = reinterpret_cast<vkh::VkViewport&>(framebuffer->viewport);
        const auto& renderArea = reinterpret_cast<vkh::VkRect2D&>(framebuffer->scissor);
        auto device = this->driver->getDeviceDispatch();
        glm::uvec4 meta = vect0;
        const uint32_t zero = 0u;

        // 
        for (uint32_t i=0;i<5;i++) {
            (*this->counters)[i] = zero;
        //    this->counters->setCommand(currentCmd);
        };

        // nullify counters
        device->CmdUpdateBuffer(currentCmd, this->counters->getGpuBuffer(), 0u, 5ull * sizeof(uint32_t), &zero);
        vkt::commandBarrier(device, currentCmd);

        {   // Denoise diffuse data
            device->CmdBindPipeline(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, this->generation);
            device->CmdBindDescriptorSets(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, layout->pipelineLayout, 0u, layout->bound.size(), layout->bound.data(), 0u, nullptr);
            device->CmdPushConstants(currentCmd, layout->pipelineLayout, layout->stages, 0u, sizeof(glm::uvec4), &meta);
            device->CmdDispatch(currentCmd, vkt::tiled(renderArea.extent.width, 32u), vkt::tiled(renderArea.extent.height, 24u), 1u);
            vkt::commandBarrier(device, currentCmd);
        };

        // TODO: Using Compute Shader for Indirect Operations
        std::vector<vkh::VkBufferCopy> regions = {
            { .srcOffset = 0ull                   , .dstOffset = 1ull * sizeof(uint32_t), .size = sizeof(uint32_t) },
            { .srcOffset = 2ull * sizeof(uint32_t), .dstOffset = 3ull * sizeof(uint32_t), .size = sizeof(uint32_t) }
        };
        device->CmdCopyBuffer(currentCmd, this->counters->getGpuBuffer(), this->counters->getGpuBuffer(), regions.size(), reinterpret_cast<const VkBufferCopy*>(regions.data()));
        vkt::commandBarrier(this->driver->getDeviceDispatch(), currentCmd);

        // 
        for (uint32_t i=0;i<3;i++) {
            {   // Intersect All
                device->CmdBindPipeline(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, this->intersection);
                device->CmdBindDescriptorSets(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, layout->pipelineLayout, 0u, layout->bound.size(), layout->bound.data(), 0u, nullptr);
                device->CmdPushConstants(currentCmd, layout->pipelineLayout, layout->stages, 0u, sizeof(glm::uvec4), &meta);
                device->CmdDispatch(currentCmd, 256u, 1u, 1u); // Planned Indirect
                vkt::commandBarrier(device, currentCmd);
                this->swapRayData(); // After intersection, needs prepare for newer rays
            };

            {   // Re-Setting Counters
                // TODO: Using Compute Shader for Indirect Operations
                device->CmdCopyBuffer(currentCmd, this->counters->getGpuBuffer(), this->counters->getGpuBuffer(), regions.size(), reinterpret_cast<const VkBufferCopy*>(regions.data()));
                vkt::commandBarrier(device, currentCmd); // TODO: Advanced Barrier
                device->CmdUpdateBuffer(currentCmd, this->counters->getGpuBuffer(), 0ull                   , sizeof(uint32_t), &zero); // Reset Ray Counter
                device->CmdUpdateBuffer(currentCmd, this->counters->getGpuBuffer(), 2ull * sizeof(uint32_t), sizeof(uint32_t), &zero); // Reset Hit Counter
                vkt::commandBarrier(device, currentCmd); // TODO: Advanced Barrier
            };

            {   // Interpolate and Re-Generate Rays
                device->CmdBindPipeline(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, this->interpolation);
                device->CmdBindDescriptorSets(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, layout->pipelineLayout, 0u, layout->bound.size(), layout->bound.data(), 0u, nullptr);
                device->CmdPushConstants(currentCmd, layout->pipelineLayout, layout->stages, 0u, sizeof(glm::uvec4), &meta);
                device->CmdDispatch(currentCmd, 256u, 1u, 1u); // Planned Indirect
                vkt::commandBarrier(device, currentCmd);
            };
        };

        {   // Resample Previous Frame
            device->CmdBindPipeline(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, this->resample);
            device->CmdBindDescriptorSets(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, layout->pipelineLayout, 0u, layout->bound.size(), layout->bound.data(), 0u, nullptr);
            device->CmdPushConstants(currentCmd, layout->pipelineLayout, layout->stages, 0u, sizeof(glm::uvec4), &meta);
            device->CmdDispatch(currentCmd, vkt::tiled(renderArea.extent.width, 32u), vkt::tiled(renderArea.extent.height, 24u), 1u);
            vkt::commandBarrier(device, currentCmd);
        };

        {   // Finalize Ray Tracing
            device->CmdBindPipeline(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, this->finalize);
            device->CmdBindDescriptorSets(currentCmd, VK_PIPELINE_BIND_POINT_COMPUTE, layout->pipelineLayout, 0u, layout->bound.size(), layout->bound.data(), 0u, nullptr);
            device->CmdPushConstants(currentCmd, layout->pipelineLayout, layout->stages, 0u, sizeof(glm::uvec4), &meta);
            device->CmdDispatch(currentCmd, vkt::tiled(renderArea.extent.width, 32u), vkt::tiled(renderArea.extent.height, 24u), 1u);
            vkt::commandBarrier(device, currentCmd);
        };
    };

};
