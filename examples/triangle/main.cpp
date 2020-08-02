#include "../include/main.hpp"

int main() {
    glfwSetErrorCallback(error);
    glfwInit();

    // 
    if (GLFW_FALSE == glfwVulkanSupported()) {
        glfwTerminate(); return -1;
    };

    // 
    uint32_t canvasWidth = SCR_WIDTH, canvasHeight = SCR_HEIGHT; // For 3840x2160 Resolutions
    //uint32_t canvasWidth = 1920, canvasHeight = 1080;
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // DEBUG!
    //std::cout << "GLFW-Hint Initialized" << std::endl;
    //system("PAUSE");

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // Ininitialize by GlBinding
    //glfwMakeContextCurrent(appObj.opengl = glfwCreateWindow(canvasWidth, canvasHeight, "GLTest", nullptr, nullptr));
#ifdef ENABLE_OPENGL_INTEROP
    glbinding::initialize(0, glfwGetProcAddress, true, false);
    glbinding::aux::enableGetErrorCallback();

    // Pravoslavie Smerti
    int flags = 0u; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & int(GL_CONTEXT_FLAG_DEBUG_BIT)) {
        // initialize debug output 
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glCheckError();
    };
#endif

    // 
    float xscale = 1.f, yscale = 1.f;
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    glfwGetMonitorContentScale(primary, &xscale, &yscale);

    // 
    auto renderArea = vkh::VkRect2D{ vkh::VkOffset2D{0, 0}, vkh::VkExtent2D{ uint32_t(canvasWidth / 2.f * xscale), uint32_t(canvasHeight / 2.f * yscale) } };
    auto viewport = vkh::VkViewport{ 0.0f, 0.0f, static_cast<float>(renderArea.extent.width), static_cast<float>(renderArea.extent.height), 0.f, 1.f };

    // 
    fw = std::make_shared<vlr::Driver>();
    auto instance = fw->createInstance();
    //auto manager = fw->createWindowSurface(canvasWidth, canvasHeight);
    auto manager = fw->createWindowSurface(fw->createWindowOnly(renderArea.extent.width, renderArea.extent.height, "VKTest"));
    auto physicalDevice = fw->getPhysicalDevice(0u);
    auto device = fw->createDevice(true, "./", false);
    auto swapchain = fw->createSwapchain();
    auto renderPass = fw->createRenderPass();
    auto framebuffers = fw->createSwapchainFramebuffer(swapchain, renderPass);
    auto queue = fw->getQueue();
    auto commandPool = fw->getCommandPool();
    auto allocator = fw->getAllocator();

    // 
    glfwSetFramebufferSizeCallback(manager.window, framebuffer_size_callback);
    //vkt::initializeGL(); // PentaXIL


    RENDERDOC_API_1_1_2* rdoc_api = NULL;

    // At init, on windows
    if (HMODULE mod = GetModuleHandleA("renderdoc.dll"))
    {
        pRENDERDOC_GetAPI RENDERDOC_GetAPI =
            (pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
        int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void**)&rdoc_api);
        assert(ret == 1);
    }

    /* 
    // At init, on linux/android.
    // For android replace librenderdoc.so with libVkLayer_GLES_RenderDoc.so
    if (void* mod = dlopen("librenderdoc.so", RTLD_NOW | RTLD_NOLOAD))
    {
        pRENDERDOC_GetAPI RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)dlsym(mod, "RENDERDOC_GetAPI");
        int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void**)&rdoc_api);
        assert(ret == 1);
    }
    */


    // BUT FOR NOW REQUIRED GPU BUFFERS! NOT JUST COPY DATA!
    const auto  imageUsage = vkh::VkImageUsageFlags{ .eTransferSrc = 1, .eTransferDst = 1, .eSampled = 1, .eStorage = 1, .eColorAttachment = 1 };
    const auto  bufferUsage = vkh::VkBufferUsageFlags{ .eTransferSrc = 1, .eTransferDst = 1, .eUniformTexelBuffer = 1, .eStorageTexelBuffer = 1, .eUniformBuffer = 1, .eStorageBuffer = 1, .eIndexBuffer = 1, .eVertexBuffer = 1, .eTransformFeedbackBuffer = 1 };
    const auto& uploadUsage = bufferUsage;

    // 
    auto bflgs = vkh::VkBufferUsageFlags{};
    vkt::unlock32(bflgs) = 0u;

    // 
    auto allocInfo = vkt::MemoryAllocationInfo{};
    allocInfo.device = fw->getDevice();
    allocInfo.memoryProperties = fw->getMemoryProperties().memoryProperties;
    allocInfo.instanceDispatch = fw->getInstanceDispatch();
    allocInfo.deviceDispatch = fw->getDeviceDispatch();

    // 
    auto aspect = vkh::VkImageAspectFlags{ .eColor = 1 };
    auto apres = vkh::VkImageSubresourceRange{ .aspectMask = aspect };

    // 
    auto constants = std::make_shared<vlr::Constants>(fw, vlr::DataSetCreateInfo{ .count = 1u, .uniform = true });
    auto bindings = std::make_shared<vlr::BindingSet>(fw, vlr::DataSetCreateInfo{ .count = 1u });
    auto accessors = std::make_shared<vlr::AttributeSet>(fw, vlr::DataSetCreateInfo{ .count = 4u });
    auto buffers = std::make_shared<vlr::BufferViewSet>(fw); 

    // 
    auto vertexSet = std::make_shared<vlr::VertexSet>(fw, vlr::VertexSetCreateInfo{
        .bindings = bindings,
        .attributes = accessors,
        .bufferViews = buffers
    });

    // 
    auto gdesc = vlr::GeometryDesc{
        .primitiveCount = 1u,
        .vertexAttribute = 0u
    };

    // use attributes
    gdesc.attributes[0u] = 1u;
    gdesc.attributes[1u] = 2u;
    gdesc.attributes[2u] = 3u;

    //
    auto vertexData = std::make_shared<vlr::SetBase_T<FDStruct>>(fw, vlr::DataSetCreateInfo{ .count = 3u });
    buffers->pushBufferView(vertexData->getGpuBuffer());

    // 
    auto geometrySet = std::make_shared<vlr::GeometrySet>(vertexSet, vlr::DataSetCreateInfo{ .count = 1u });
    auto geometry = std::make_shared<vlr::Geometry>(vertexSet, gdesc);
    

    // 
    vertexData->get(0u) = FDStruct{ .fPosition = glm::vec4( 1.f, -1.f, 0.f, 1.f), .fNormal = glm::vec4(0.f, 0.f, 1.f, 0.f), .fTangent = glm::vec4(0.f, 1.f, 0.f, 0.f) };
    vertexData->get(1u) = FDStruct{ .fPosition = glm::vec4(-1.f, -1.f, 0.f, 1.f), .fNormal = glm::vec4(0.f, 0.f, 1.f, 0.f), .fTangent = glm::vec4(0.f, 1.f, 0.f, 0.f) };
    vertexData->get(2u) = FDStruct{ .fPosition = glm::vec4( 0.f,  1.f, 0.f, 1.f), .fNormal = glm::vec4(0.f, 0.f, 1.f, 0.f), .fTangent = glm::vec4(0.f, 1.f, 0.f, 0.f) };

    // 
    bindings->get(0u) = vkh::VkVertexInputBindingDescription{
        .binding = 0u,
        .stride = sizeof(FDStruct)
    };

    // 
    accessors->get(0u) = vkh::VkVertexInputAttributeDescription{
        .location = 0u, .binding = 0u,
        .format = VK_FORMAT_R32G32B32A32_SFLOAT,
        .offset = offsetof(FDStruct, fPosition)
    };

    accessors->get(1u) = vkh::VkVertexInputAttributeDescription{
        .location = 1u, .binding = 0u,
        .format = VK_FORMAT_R32G32B32A32_SFLOAT,
        .offset = offsetof(FDStruct, fTexcoord)
    };

    accessors->get(2u) = vkh::VkVertexInputAttributeDescription{
        .location = 2u, .binding = 0u,
        .format = VK_FORMAT_R32G32B32A32_SFLOAT,
        .offset = offsetof(FDStruct, fNormal)
    };

    accessors->get(3u) = vkh::VkVertexInputAttributeDescription{
        .location = 3u, .binding = 0u,
        .format = VK_FORMAT_R32G32B32A32_SFLOAT,
        .offset = offsetof(FDStruct, fTangent)
    };

    // 
    geometrySet->pushGeometry(geometry);

    // 
    fw->submitOnce([&](VkCommandBuffer cmd) {
        vertexData->setCommand(cmd);
        bindings->setCommand(cmd);
        accessors->setCommand(cmd);
        geometrySet->setCommand(cmd);
    });

    // 
    auto instanceSet = std::make_shared<vlr::InstanceSet>(fw, vlr::DataSetCreateInfo{ .count = 1u });
    auto accelerationBottom = std::make_shared<vlr::Acceleration>(fw, vlr::AccelerationCreateInfo{ .geometrySet = geometrySet, .initials = {1u} });
    auto accelerationTop = std::make_shared<vlr::Acceleration>(fw, vlr::AccelerationCreateInfo{ .instanceSet = instanceSet, .initials = {1u} });
    auto framebuffer = std::make_shared<vlr::Framebuffer>(fw);
    auto layout = std::make_shared<vlr::PipelineLayout>(fw);

    // 
    const auto testInstance = vkh::VsGeometryInstance{
        .transform = glm::mat3x4(1.f),
        .customId = 0u,
        .mask = 0xFFu,
        .instanceOffset = 0u,
        .flags = VK_GEOMETRY_INSTANCE_TRIANGLE_CULL_DISABLE_BIT_NV,
        .accelerationStructureHandle = accelerationBottom->getHandle()
    };
    instanceSet->get(0u) = testInstance;

    // 
    auto rasterization = std::make_shared<vlr::Rasterization>(fw, vlr::PipelineCreateInfo{
        .layout = layout,
        .framebuffer = framebuffer,
        .instanceSet = instanceSet,
        .constants = constants,
        .geometrySets = {geometrySet},
    });

    // 
    auto rayTracing = std::make_shared<vlr::RayTracing>(fw, vlr::RayTracingCreateInfo{
        .layout = layout,
        .framebuffer = framebuffer,
        .accelerationTop = accelerationTop,
        .constants = constants,
    });

    //
    auto renderCommand = std::make_shared<vlr::RenderCommand>(fw, vlr::RenderCommandCreateInfo{
        .layout = layout,
        .rayTracing = rayTracing,
        .rasterization = rasterization
    });

    // 
    auto buildCommand = std::make_shared<vlr::BuildCommand>(fw, vlr::BuildCommandCreateInfo{
        .layout = layout,
        .accelerationTop = accelerationTop,
    });

    

    // 
    auto materialSet = std::make_shared<vlr::MaterialSet>(fw, vlr::DataSetCreateInfo{ .count = 1u });
    auto textureSet = std::make_shared<vlr::TextureSet>(fw);
    auto samplerSet = std::make_shared<vlr::SamplerSet>(fw);
    auto background = std::make_shared<vlr::Background>(fw);


    // 
    auto& testMaterial = materialSet->get(0u);
    testMaterial = vlr::MaterialUnit{};
    testMaterial.diffuse = glm::vec4(1.f,1.f,1.f,1.f);


    // 
    framebuffer->createFramebuffer(canvasWidth, canvasHeight);

    {   // 
        int width = 0u, height = 0u;
        float* rgba = nullptr;
        const char* err = nullptr;

        //
        std::vector<glm::vec4> gSkyColor = {
            glm::vec4(0.9f,0.98,0.999f, 1.f),
            glm::vec4(0.9f,0.98,0.999f, 1.f),
            glm::vec4(0.9f,0.98,0.999f, 1.f),
            glm::vec4(0.9f,0.98,0.999f, 1.f)
        };

        {   //
            int ret = LoadEXR(&rgba, &width, &height, "spiaggia_di_mondello_4k.exr", &err);
            if (ret != 0) {
                printf("err: %s\n", err); //

                // backdoor image
                rgba = (float*)gSkyColor.data();
                width = 2u, height = 2u;
            }
        };

        {   //
            vkt::VmaMemoryInfo memInfo = {};
            memInfo.memUsage = VMA_MEMORY_USAGE_GPU_ONLY;

            //
            auto image = vkt::ImageRegion(std::make_shared<vkt::VmaImageAllocation>(fw->getAllocator(), vkh::VkImageCreateInfo{}.also([=](vkh::VkImageCreateInfo* it) {
                it->format = VK_FORMAT_R32G32B32A32_SFLOAT,
                it->extent = vkh::VkExtent3D{ uint32_t(width),uint32_t(height),1u },
                it->usage = imageUsage;
                return it;
            }), memInfo), vkh::VkImageViewCreateInfo{}.also([=](vkh::VkImageViewCreateInfo* it) {
                it->format = VK_FORMAT_R32G32B32A32_SFLOAT,
                it->subresourceRange = apres;
                return it;
            }));

            //
            vkt::Vector<> imageBuf = {};
            if (width > 0u && height > 0u && rgba) {
                memInfo.memUsage = VMA_MEMORY_USAGE_CPU_TO_GPU;
                imageBuf = vkt::Vector<>(std::make_shared<vkt::VmaBufferAllocation>(fw->getAllocator(), vkh::VkBufferCreateInfo{ // experimental: callify
                    .size = size_t(width) * size_t(height) * sizeof(glm::vec4), .usage = uploadUsage,
                }, memInfo));
                memcpy(imageBuf.data(), rgba, size_t(width) * size_t(height) * sizeof(glm::vec4));
            };

            //
            fw->submitOnce([&](VkCommandBuffer& cmd) {
                image.transfer(cmd);

                auto buffer = imageBuf;
                fw->getDeviceDispatch()->CmdCopyBufferToImage(cmd, buffer.buffer(), image.getImage(), image.getImageLayout(), 1u, vkh::VkBufferImageCopy{
                    .bufferOffset = buffer.offset(),
                    .bufferRowLength = uint32_t(width),
                    .bufferImageHeight = uint32_t(height),
                    .imageSubresource = image.subresourceLayers(),
                    .imageOffset = {0u,0u,0u},
                    .imageExtent = {uint32_t(width),uint32_t(height),1u},
                });
            });

            //
            background->setImage(image);
        };
    };

    {   // 
        int width = 2u, height = 2u;
        float* rgba = nullptr;
        const char* err = nullptr;

        //
        std::vector<glm::vec4> testcolor = {
            glm::vec4(1.f, 0.f, 1.f, 1.f),
            glm::vec4(0.f, 0.f, 0.f, 1.f),
            glm::vec4(0.f, 0.f, 0.f, 1.f),
            glm::vec4(1.f, 0.f, 1.f, 1.f)
        };
        rgba = (float*)testcolor.data();

        {   //
            vkt::VmaMemoryInfo memInfo = {};
            memInfo.memUsage = VMA_MEMORY_USAGE_GPU_ONLY;

            //
            auto image = vkt::ImageRegion(std::make_shared<vkt::VmaImageAllocation>(fw->getAllocator(), vkh::VkImageCreateInfo{}.also([=](vkh::VkImageCreateInfo* it) {
                it->format = VK_FORMAT_R32G32B32A32_SFLOAT,
                it->extent = vkh::VkExtent3D{ uint32_t(width),uint32_t(height),1u },
                it->usage = imageUsage;
                return it;
            }), memInfo), vkh::VkImageViewCreateInfo{}.also([=](vkh::VkImageViewCreateInfo* it) {
                it->format = VK_FORMAT_R32G32B32A32_SFLOAT,
                it->subresourceRange = apres;
                return it;
            }));

            //
            vkt::Vector<> imageBuf = {};
            if (width > 0u && height > 0u && rgba) {
                memInfo.memUsage = VMA_MEMORY_USAGE_CPU_TO_GPU;
                imageBuf = vkt::Vector<>(std::make_shared<vkt::VmaBufferAllocation>(fw->getAllocator(), vkh::VkBufferCreateInfo{ // experimental: callify
                    .size = size_t(width) * size_t(height) * sizeof(glm::vec4), .usage = uploadUsage,
                }, memInfo));
                memcpy(imageBuf.data(), rgba, size_t(width) * size_t(height) * sizeof(glm::vec4));
            };

            //
            fw->submitOnce([&](VkCommandBuffer& cmd) {
                image.transfer(cmd);

                auto buffer = imageBuf;
                fw->getDeviceDispatch()->CmdCopyBufferToImage(cmd, buffer.buffer(), image.getImage(), image.getImageLayout(), 1u, vkh::VkBufferImageCopy{
                    .bufferOffset = buffer.offset(),
                    .bufferRowLength = uint32_t(width),
                    .bufferImageHeight = uint32_t(height),
                    .imageSubresource = image.subresourceLayers(),
                    .imageOffset = {0u,0u,0u},
                    .imageExtent = {uint32_t(width),uint32_t(height),1u},
                });
            });

            // 
            textureSet->pushImage(image);
        };
    };

    {   // Test Sampler
        VkSampler sampler = {};
        fw->getDeviceDispatch()->CreateSampler(vkh::VkSamplerCreateInfo{
            .magFilter = VK_FILTER_LINEAR,
            .minFilter = VK_FILTER_LINEAR,
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT
        }, nullptr, &sampler);

        // 
        samplerSet->pushSampler(sampler);
    };


    // 
    layout->setFramebuffer(framebuffer);
    layout->setBackground(background);
    layout->setMaterials(materialSet, textureSet, samplerSet);
    layout->setVertexSet(vertexSet);

    // 


    // 
    rasterization->setDescriptorSets(layout);
    rayTracing->setDescriptorSets(layout);
    renderCommand->setDescriptorSets(layout);
    buildCommand->setDescriptorSets(layout);





    // 
    //glm::dvec3 eye = glm::dvec3(5.f, 2.f, 2.f);
    glm::dvec3 eye = glm::dvec3(2.f, 2.f, 5.f);
    glm::dvec3 foc = glm::dvec3(0.f, 0.f, 0.f);
    glm::dvec3 evc = foc - eye;
    glm::dvec3 upv = glm::dvec3(0.f, 1.f, 0.f);
    glm::uvec2 canvasSize = { canvasWidth, canvasHeight };

    // 
    auto cameraController = std::make_shared<CameraController>();
    cameraController->canvasSize = &canvasSize;
    cameraController->eyePos = &eye;
    cameraController->upVector = &upv;
    cameraController->viewVector = &evc;

    // 
    vkh::VsGraphicsPipelineCreateInfoConstruction pipelineInfo = {};
    pipelineInfo.stages = {
        vkt::makePipelineStageInfo(fw->getDeviceDispatch(), vkt::readBinary(std::string("./shaders/render.vert.spv")), VK_SHADER_STAGE_VERTEX_BIT),
        vkt::makePipelineStageInfo(fw->getDeviceDispatch(), vkt::readBinary(std::string("./shaders/render.frag.spv")), VK_SHADER_STAGE_FRAGMENT_BIT)
    };
    pipelineInfo.graphicsPipelineCreateInfo.layout = layout->getPipelineLayout();
    pipelineInfo.graphicsPipelineCreateInfo.renderPass = fw->applicationWindow.renderPass;//context->refRenderPass();
    pipelineInfo.inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    pipelineInfo.viewportState.pViewports = &reinterpret_cast<vkh::VkViewport&>(viewport);
    pipelineInfo.viewportState.pScissors = &reinterpret_cast<vkh::VkRect2D&>(renderArea);
    pipelineInfo.colorBlendAttachmentStates = { {} }; // Default Blend State
    pipelineInfo.dynamicStates = { VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_VIEWPORT };

    // 
    VkPipeline finalPipeline = {};
    vkh::handleVk(fw->getDeviceDispatch()->CreateGraphicsPipelines(fw->getPipelineCache(), 1u, pipelineInfo, nullptr, &finalPipeline));

    // 
    int64_t currSemaphore = -1;
    uint32_t currentBuffer = 0u;
    uint32_t frameCount = 0u;

    // set GLFW callbacks
    glfwSetMouseButtonCallback(fw->window(), &Shared::MouseButtonCallback);
    glfwSetCursorPosCallback(fw->window(), &Shared::MouseMoveCallback);
    glfwSetKeyCallback(fw->window(), &Shared::KeyCallback);

    // 
    Shared::active = Active{};
    Shared::window = fw->window(); // set GLFW window
    Shared::active.tDiff = 0.0; // reset diff to near-zero (avoid critical math errors)
    Shared::active.keys.resize(1024, uint8_t(0u));
    Shared::active.mouse.resize(128, uint8_t(0u));
    Shared::TimeCallback(glfwGetTime() * 1000.0);

    // 
    auto counters = rayTracing->getCounters();

    // 
    while (!glfwWindowShouldClose(manager.window)) { // 
        glfwPollEvents();

        // 
        if (rdoc_api) rdoc_api->StartFrameCapture(NULL, NULL);

        // 
        int64_t n_semaphore = currSemaphore, c_semaphore = (currSemaphore + 1) % framebuffers.size(); // Next Semaphore
        currSemaphore = (c_semaphore = c_semaphore >= 0 ? c_semaphore : int64_t(framebuffers.size()) + c_semaphore); // Current Semaphore
        (n_semaphore = n_semaphore >= 0 ? n_semaphore : int64_t(framebuffers.size()) + n_semaphore); // Fix for Next Semaphores

        // 
        vkh::handleVk(fw->getDeviceDispatch()->WaitForFences(1u, &framebuffers[c_semaphore].waitFence, true, 30ull * 1000ull * 1000ull * 1000ull));
        vkh::handleVk(fw->getDeviceDispatch()->ResetFences(1u, &framebuffers[c_semaphore].waitFence));
        vkh::handleVk(fw->getDeviceDispatch()->AcquireNextImageKHR(swapchain, std::numeric_limits<uint64_t>::max(), framebuffers[c_semaphore].presentSemaphore, nullptr, &currentBuffer));
        //fw->getDeviceDispatch()->SignalSemaphore(vkh::VkSemaphoreSignalInfo{.semaphore = framebuffers[n_semaphore].semaphore, .value = 1u});

        // 
        std::vector<uint32_t> cdata = { counters->get(0u), counters->get(1u), counters->get(2u), counters->get(3u), counters->get(4u) };

        { // submit rendering (and wait presentation in device)
            vkh::VkClearValue clearValues[2] = { {}, {} };
            clearValues[0].color = vkh::VkClearColorValue{}; clearValues[0].color.float32 = glm::vec4(0.f, 0.f, 0.f, 0.f);
            clearValues[1].depthStencil = VkClearDepthStencilValue{ 1.0f, 0 };

            // 
            Shared::TimeCallback(glfwGetTime()*1000.0);
            glm::mat4 proj = cameraController->handle().project();

            // 
            constants->get(0u).modelview = glm::transpose(glm::mat4x3(proj));
            constants->get(0u).projection = glm::transpose(glm::mat4x4(glm::perspective(80.f / 180.f * glm::pi<double>(), double(canvasWidth) / double(canvasHeight), 0.001, 10000.)));
            constants->get(0u).modelviewInv = glm::transpose(glm::mat4x3(glm::inverse(proj)));
            constants->get(0u).projectionInv = glm::transpose(glm::mat4x4(glm::inverse(glm::perspective(80.f / 180.f * glm::pi<double>(), double(canvasWidth) / double(canvasHeight), 0.001, 10000.))));

            // Create render submission 
            std::vector<VkSemaphore> waitSemaphores = { framebuffers[currentBuffer].presentSemaphore }, signalSemaphores = { framebuffers[currentBuffer].computeSemaphore };
            std::vector<vkh::VkPipelineStageFlags> waitStages = {
                vkh::VkPipelineStageFlags{.eFragmentShader = 1, .eComputeShader = 1, .eTransfer = 1, .eRayTracingShader = 1, .eAccelerationStructureBuild = 1 },
                vkh::VkPipelineStageFlags{.eFragmentShader = 1, .eComputeShader = 1, .eTransfer = 1, .eRayTracingShader = 1, .eAccelerationStructureBuild = 1 }
            };

            // 
            auto rtCommand = vkt::createCommandBuffer(fw->getDeviceDispatch(), commandPool, false, false);
            instanceSet->setCommand(rtCommand);
            materialSet->setCommand(rtCommand);
            constants->setCommand(rtCommand, true);
            buildCommand->setCommand(rtCommand);
            renderCommand->setCommand(rtCommand);
            vkt::commandBarrier(fw->getDeviceDispatch(), rtCommand);

            // 
            fw->getDeviceDispatch()->CmdCopyBuffer(rtCommand, counters->getGpuBuffer(), counters->getCpuBuffer(), 1u, vkh::VkBufferCopy{
                counters->getGpuBuffer().offset(),
                counters->getCpuBuffer().offset(),
                counters->getCpuBuffer().range()
            });


            fw->getDeviceDispatch()->EndCommandBuffer(rtCommand);
            //break; // FOR DEBUG!!

            // Submit command once
            //renderer->setupCommands();
            vkh::handleVk(fw->getDeviceDispatch()->QueueSubmit(queue, 1u, vkh::VkSubmitInfo{
                .waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size()), .pWaitSemaphores = waitSemaphores.data(), .pWaitDstStageMask = waitStages.data(),
                .commandBufferCount = 1u, .pCommandBuffers = &rtCommand,
                .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()), .pSignalSemaphores = signalSemaphores.data()
            }, {}));

            // 
            waitSemaphores = { framebuffers[currentBuffer].computeSemaphore }, signalSemaphores = { framebuffers[currentBuffer].drawSemaphore };

            // create command buffer (with rewrite)
            VkCommandBuffer& commandBuffer = framebuffers[currentBuffer].commandBuffer;
            if (!commandBuffer) {
                commandBuffer = vkt::createCommandBuffer(fw->getDeviceDispatch(), commandPool, false, false); // do reference of cmd buffer

                //
                decltype(auto) descriptorSets = layout->getDescriptorSets();

                // Reuse depth as general
                vkt::imageBarrier(commandBuffer, vkt::ImageBarrierInfo{
                    .image = fw->getDepthImage(),
                    .targetLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                    .originLayout = VK_IMAGE_LAYOUT_GENERAL,
                    .subresourceRange = vkh::VkImageSubresourceRange{ {}, 0u, 1u, 0u, 1u }.also([=](auto* it) {
                        auto aspect = vkh::VkImageAspectFlags{.eDepth = 1u, .eStencil = 1u };
                        it->aspectMask = aspect;
                        return it;
                    })
                });

                // 
                fw->getDeviceDispatch()->CmdBeginRenderPass(commandBuffer, vkh::VkRenderPassBeginInfo{ .renderPass = fw->applicationWindow.renderPass, .framebuffer = framebuffers[currentBuffer].frameBuffer, .renderArea = renderArea, .clearValueCount = 2u, .pClearValues = reinterpret_cast<vkh::VkClearValue*>(&clearValues[0]) }, VK_SUBPASS_CONTENTS_INLINE);
                fw->getDeviceDispatch()->CmdSetViewport(commandBuffer, 0u, 1u, viewport);
                fw->getDeviceDispatch()->CmdSetScissor(commandBuffer, 0u, 1u, renderArea);
                fw->getDeviceDispatch()->CmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, finalPipeline);
                fw->getDeviceDispatch()->CmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout->getPipelineLayout(), 0u, descriptorSets.size(), descriptorSets.data(), 0u, nullptr);
                fw->getDeviceDispatch()->CmdDraw(commandBuffer, 4, 1, 0, 0);
                fw->getDeviceDispatch()->CmdEndRenderPass(commandBuffer);
                vkt::commandBarrier(fw->getDeviceDispatch(), commandBuffer);

                // Reuse depth as general
                vkt::imageBarrier(commandBuffer, vkt::ImageBarrierInfo{
                    .image = fw->getDepthImage(),
                    .targetLayout = VK_IMAGE_LAYOUT_GENERAL,
                    .originLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                    .subresourceRange = vkh::VkImageSubresourceRange{ {}, 0u, 1u, 0u, 1u }.also([=](auto* it) {
                        auto aspect = vkh::VkImageAspectFlags{.eDepth = 1u, .eStencil = 1u };
                        it->aspectMask = aspect;
                        return it;
                    })
                });

                // Use as present image
                vkt::imageBarrier(commandBuffer, vkt::ImageBarrierInfo{
                    .image = framebuffers[currentBuffer].image,
                    .targetLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                    .originLayout = VK_IMAGE_LAYOUT_GENERAL,
                    .subresourceRange = vkh::VkImageSubresourceRange{ {}, 0u, 1u, 0u, 1u }.also([=](auto* it) {
                        auto aspect = vkh::VkImageAspectFlags{ .eColor = 1u };
                        it->aspectMask = aspect;
                        return it;
                    })
                });

                // 
                fw->getDeviceDispatch()->EndCommandBuffer(commandBuffer);
            };

            // Submit command once
            vkh::handleVk(fw->getDeviceDispatch()->QueueSubmit(queue, 1u, vkh::VkSubmitInfo{
                .waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size()), .pWaitSemaphores = waitSemaphores.data(), .pWaitDstStageMask = waitStages.data(),
                .commandBufferCount = 1u, .pCommandBuffers = &commandBuffer,
                .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()), .pSignalSemaphores = signalSemaphores.data()
            }, framebuffers[currentBuffer].waitFence));
            
            // 
            constants->get(0u).rdata.x = frameCount++;
        };

        // 
        std::vector<VkSemaphore> waitSemaphoes = { framebuffers[c_semaphore].drawSemaphore };
        vkh::handleVk(fw->getDeviceDispatch()->QueuePresentKHR(queue, vkh::VkPresentInfoKHR{
            .waitSemaphoreCount = static_cast<uint32_t>(waitSemaphoes.size()), .pWaitSemaphores = waitSemaphoes.data(),
            .swapchainCount = 1, .pSwapchains = &swapchain,
            .pImageIndices = &currentBuffer, .pResults = nullptr
        }));

        // 
        if (rdoc_api) rdoc_api->EndFrameCapture(NULL, NULL);
    };

    // 
    glfwDestroyWindow(manager.window);
    glfwTerminate(); exit(0);
    return 0;
};
