#pragma once
#include "./Config.hpp"
#include <vkt3/fw.hpp>

namespace vlr {

    using Driver = vkt::GPUFramework;

};

//#include "./SetBase.hpp"

namespace vlj {
    class Driver : public Wrap<vlr::Driver> {
    public:
        //Driver() : Wrap<vlr::Driver>() {};
        Driver(vkt::uni_ptr<vlr::Driver> object) : Wrap<vlr::Driver>(object) {};
        Driver() : Wrap<vlr::Driver>(std::make_shared<vlr::Driver>()) {};
        Driver(std::shared_ptr<vlr::Driver> object) : Wrap<vlr::Driver>(object) {};

        // For Java
        virtual vlr::Driver* submitCmd(std::vector<int64_t> cmds, vkt::uni_arg<vkh::VkSubmitInfo> smbi = vkh::VkSubmitInfo{}) {
            return object->submitCmd(vkt::vector_cast<VkCommandBuffer>(cmds), smbi);
        };

        // 
        virtual vlr::Driver* submitCmd(vkt::uni_arg<VkCommandBuffer> cmds, vkt::uni_arg<vkh::VkSubmitInfo> smbi = vkh::VkSubmitInfo{}) {
            return object->submitCmd(cmds, smbi);
        };

        // For Java
        virtual vlr::Driver* submitUtilize(std::vector<int64_t> cmds, vkt::uni_arg<vkh::VkSubmitInfo> smbi = vkh::VkSubmitInfo{}) {
            return object->submitUtilize(vkt::vector_cast<VkCommandBuffer>(cmds), smbi);
        };

        // 
        virtual vlr::Driver* submitUtilize(vkt::uni_arg<VkCommandBuffer> cmds, vkt::uni_arg<vkh::VkSubmitInfo> smbi = vkh::VkSubmitInfo{}) {
            return object->submitUtilize(cmds, smbi);
        };

        //CALLIFY(constructor);
        CALLIFY(getPhysicalDevice);
        CALLIFY(getDeviceDispatch);
        CALLIFY(getInstanceDispatch);
        CALLIFY(getDevice);
        CALLIFY(getQueue);
        CALLIFY(getFence);
        //CALLIFY(getRenderPass);
        CALLIFY(getInstance);
        CALLIFY(getCommandPool);
        CALLIFY(getPipelineCache);
        CALLIFY(getDescriptorPool);
        CALLIFY(getMemoryProperties);
        CALLIFY(getAllocator);
        //CALLIFY(submitUtilize);
        //CALLIFY(submitCmd);
        //CALLIFY(submitCommandWithSync);
        //CALLIFY(getDepthImageView);
        CALLIFY(getDepthImage);
        CALLIFY(createInstance);
        CALLIFY(createDevice);
        CALLIFY(getInstanceCreateInfoAddress);
        CALLIFY(getDeviceCreateInfoAddress);
        CALLIFY(memoryAllocationInfo);
        CALLIFY(memoryAllocationInfoPtr);
        CALLIFY(loadXVK);
        CALLIFY(getDepthImageView);
        //CALLIFY(initializeGL);

#ifdef VKT_ENABLE_GLFW_SUPPORT
        CALLIFY(createWindowSurface);
        CALLIFY(surface);
        CALLIFY(window);
        CALLIFY(format);
        CALLIFY(size);
        CALLIFY(getSurfaceFormat);
        CALLIFY(createRenderPass);
        CALLIFY(updateSwapchainFramebuffer);
        CALLIFY(createSwapchainFramebuffer);
        CALLIFY(createSwapchain);
        CALLIFY(getSwapchain);
        CALLIFY(getSurface);
        CALLIFY(getRenderPass);
#endif
    };
};
