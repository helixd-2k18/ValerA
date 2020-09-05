#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./PipelineLayout.hpp"

#ifdef ENABLE_OPTIX_DENOISE
#include "./OptiXDenoise.hpp"
#endif

namespace vlr {

    struct PipelineStages {
        std::string vertexShader = "./shaders/rasterize.vert.spv";
        std::string geometryShader = "./shaders/rasterize.geom.spv";
        std::string fragmentShader = "./shaders/rasterize.frag.spv";
    };

    struct PipelineCreateInfo {
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<Framebuffer> framebuffer = {};
        vkt::uni_ptr<InstanceSet> instanceSet = {};
        vkt::uni_ptr<Constants> constants = {};
        PipelineStages translucent = {
            .vertexShader = "./shaders/rasterize.vert.spv",
            .geometryShader = "./shaders/rasterize.geom.spv",
            .fragmentShader = "./shaders/rasterize.frag.spv"
        };
        PipelineStages opaque = {
            .vertexShader = "./shaders/opaque.vert.spv",
            .geometryShader = "./shaders/opaque.geom.spv",
            .fragmentShader = "./shaders/opaque.frag.spv"
        };
    };

    struct RayTracingCreateInfo {
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<Framebuffer> framebuffer = {};
        vkt::uni_ptr<Acceleration> accelerationTop = {}; // Top Level for Ray Tracing
        vkt::uni_ptr<Constants> constants = {};
        std::string generationShader = "./shaders/generation.comp.spv";
        std::string intersectionShader = "./shaders/intersection.comp.spv";
        std::string interpolationShader = "./shaders/interpolation.comp.spv";
        std::string resampleShader = "./shaders/resample.comp.spv";
        std::string finalizeShader = "./shaders/finalize.comp.spv";
        std::string cbfixShader = "./shaders/cbfix.comp.spv";
        std::string compositeShader = "./shaders/composite.comp.spv";
#ifdef ENABLE_OPTIX_DENOISE
        vkt::uni_ptr<OptiXDenoise> denoise = {};
#endif
    };

    struct RenderPass {
        VkFramebuffer framebuffer = {};
        VkRenderPass renderPass = {};
        
        // 
        std::vector<vkh::VkPipelineColorBlendAttachmentState> blendStates = {};
        std::vector<vkh::VkClearValue> clearValues = {};
        vkh::VsDescriptorSetCreateInfoHelper descriptorSetInfo = {};

        // 
        RenderPass* setBlendStates(vkh::VkPipelineColorBlendAttachmentState* ptr, int32_t size = 1u) {
            blendStates.resize(size);
            memcpy(blendStates.data(), ptr, size * sizeof(vkh::VkPipelineColorBlendAttachmentState));
            return this;
        };

        // 
        RenderPass* setClearValues(vkh::VkClearValue* ptr, int32_t size = 1u) {
            clearValues.resize(size);
            memcpy(clearValues.data(), ptr, size * sizeof(vkh::VkClearValue));
            return this;
        };
    };

    class Framebuffer : public std::enable_shared_from_this<Framebuffer> { protected: friend Rasterization; friend Resampling; friend RayTracing; friend PipelineLayout;
#ifdef ENABLE_OPTIX_DENOISE
    friend OptiXDenoise;
#endif

        RenderPass rasterFBO = {}, resampleFBO = {}; VkDescriptorSet set = {};
        vkt::ImageRegion depthStencilImage = {}, atomicMapping = {};
        std::vector<VkSampler> samplers = {};
        std::vector<vkt::ImageRegion> currentsImages = {};  // Current Frame
        std::vector<vkt::ImageRegion> previousImages = {};  // Previous Frame
        std::vector<vkt::ImageRegion> resampleImages = {}; // Resampled Previous Frame
        std::vector<vkt::ImageRegion> rasterImages = {};    // Rasterized Frame
        std::vector<vkt::ImageRegion> inoutLinearImages = {};
        VkRect2D scissor = {}; VkViewport viewport = {};    // 
        uint32_t width = 1920u, height = 1200u;
        vkt::uni_ptr<Driver> driver = {};
        bool updated = false;

    public: 
        Framebuffer() { this->constructor(); };
        Framebuffer(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };
        ~Framebuffer() {};

        // 
        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver);
        virtual void createRenderPass();
        virtual void createFramebuffer(uint32_t width = 1920u, uint32_t height = 1200u);
        virtual void createDescriptorSet(vkt::uni_ptr<PipelineLayout> layout);

        // 
        virtual void imageToLinearCopyCommand(vkt::uni_arg<VkCommandBuffer> cmd, const uint32_t& whatDenoise = 4u);
        virtual void linearToImageCopyCommand(vkt::uni_arg<VkCommandBuffer> cmd, const uint32_t& whatDenoise = 4u);
    };
};

namespace vlj {
    class Framebuffer : public Wrap<vlr::Framebuffer> {
    public:
        Framebuffer() : Wrap<vlr::Framebuffer>() {};
        Framebuffer(vkt::uni_ptr<vlr::Framebuffer> object) : Wrap<vlr::Framebuffer>(object) {};
        Framebuffer(vkt::uni_ptr<vlr::Driver> driver) : Wrap<vlr::Framebuffer>(std::make_shared<vlr::Framebuffer>(driver)) {};
        Framebuffer(std::shared_ptr<vlr::Framebuffer> object) : Wrap<vlr::Framebuffer>(object) {};

        //CALLIFY(constructor);
        CALLIFY(createRenderPass);
        CALLIFY(createFramebuffer);
        CALLIFY(createDescriptorSet);

        // 
        CALLIFY(imageToLinearCopyCommand);
        CALLIFY(linearToImageCopyCommand);
    };
};
