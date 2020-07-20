#include "./vlr/SetBase.hpp"
#include "./vlr/PipelineLayout.hpp"
#include "./vlr/Driver.hpp"

namespace vlr {

    void SetBase::constructor(vkt::uni_ptr<Driver> driver, const uint32_t& stride, vkt::uni_arg<DataSetCreateInfo> info) {
        auto hostUsage = vkh::VkBufferUsageFlags{.eTransferSrc = 1, .eTransferDst = 1, .eUniformBuffer = 1, .eStorageBuffer = 1, .eRayTracing = 1 };
        auto gpuUsage = vkh::VkBufferUsageFlags{.eTransferSrc = 1, .eTransferDst = 1, .eUniformBuffer = 1, .eStorageBuffer = 1, .eRayTracing = 1 };
        auto upstreamUsage = vkh::VkBufferUsageFlags{ .eTransferSrc = 1, .eTransferDst = 1, .eUniformBuffer = 1, .eStorageBuffer = 1, .eIndirectBuffer = 1, .eRayTracing = 1, .eTransformFeedbackBuffer = 1, .eTransformFeedbackCounterBuffer = 1, .eSharedDeviceAddress = 1 };

        // 
        auto allocator = this->driver->getAllocator();
        this->getCpuBuffer() = vkt::VectorBase(std::make_shared<vkt::VmaBufferAllocation>(allocator, vkh::VkBufferCreateInfo{ .size = sizeof(T) * info->count, .usage = hostUsage }, vkt::VmaMemoryInfo{ .memUsage = VMA_MEMORY_USAGE_CPU_TO_GPU }), 0ull, sizeof(T) * info->count, stride);
        this->getGpuBuffer() = vkt::VectorBase(std::make_shared<vkt::VmaBufferAllocation>(allocator, vkh::VkBufferCreateInfo{ .size = sizeof(T) * info->count, .usage =  gpuUsage }, vkt::VmaMemoryInfo{ .memUsage = VMA_MEMORY_USAGE_GPU_ONLY }), 0ull, sizeof(T) * info->count, stride);
    };

    void SetBase::createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) {
        this->descriptorSetInfo = vkh::VsDescriptorSetCreateInfoHelper(pipelineLayout->getSetLayout(), pipelineLayout->getDescriptorPool());
        auto& handle = this->descriptorSetInfo.pushDescription(vkh::VkDescriptorUpdateTemplateEntry{
            .dstBinding = 0u,
            .dstArrayElement = 0u,
            .descriptorCount = 1u,
            .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
        });
        handle.offset<VkDescriptorBufferInfo>(0) = this->gpuBuffer;
        vkh::handleVk(vkt::AllocateDescriptorSetWithUpdate(driver->getDeviceDispatch(), this->descriptorSetInfo, this->set, this->updated));
    };

    void SetBase::setCommand(vkt::uni_arg<VkCommandBuffer> commandBuffer, bool barrier) {
        auto device = driver->getDeviceDispatch();
        vkh::VkBufferCopy region = { .srcOffset = 0ull, .dstOffset = 0ull, .size = this->getGpuBuffer().range() };
        device->CmdCopyBuffer(commandBuffer, this->getCpuBuffer(), this->getGpuBuffer(), 1, region);
        if (barrier) { vkt::commandBarrier(device, commandBuffer); }; // TODO: Advanced Barrier
    };

};
