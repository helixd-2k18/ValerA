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

    /*
    RENDERDOC_API_1_1_2* rdoc_api = NULL;

    // At init, on windows
    if (HMODULE mod = GetModuleHandleA("renderdoc.dll"))
    {
        pRENDERDOC_GetAPI RENDERDOC_GetAPI =
            (pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
        int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void**)&rdoc_api);
        assert(ret == 1);
    }*/

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


    // 
    std::string err = "", wrn = "";
    tinygltf::Model model = {};
    tinygltf::TinyGLTF loader = {};

    // 
    const float unitScale = 100.f, unitHeight = -0.f;
    const bool ret = loader.LoadASCIIFromFile(&model, &err, &wrn, "BoomBoxWithAxes.gltf");

    // 
    //const float unitScale = 100.f, unitHeight = -0.f;
    //const bool ret = loader.LoadASCIIFromFile(&model, &err, &wrn, "Box.gltf");

    // 
    //const float unitScale = 1.f, unitHeight = -32.f;
    //const bool ret = loader.LoadASCIIFromFile(&model, &err, &wrn, "lost_empire.gltf"); // (May) have VMA memory issues

    //const float unitScale = 1.f, unitHeight = -0.f;
    //const bool ret = loader.LoadASCIIFromFile(&model, &err, &wrn, "Chess_Set.gltf");
    //const bool ret = loader.LoadASCIIFromFile(&model, &err, &wrn, "DamagedHelmet.gltf");

    //const bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)

    //
    if (!wrn.empty()) { printf("Warn : %s\n", wrn.c_str()); }
    if (!err.empty()) { printf("Error: %s\n", err.c_str()); }
    if (!ret) { printf("Failed to parse glTF\n"); return -1; }



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
    auto bindings = std::make_shared<vlr::BindingSet>(fw, vlr::DataSetCreateInfo{ .count = model.bufferViews.size() });
    auto accessors = std::make_shared<vlr::AttributeSet>(fw, vlr::DataSetCreateInfo{ .count = model.accessors.size() });
    auto buffers = std::make_shared<vlr::BufferViewSet>(fw); 


    //buffers->pushBufferView(vertexData->getGpuBuffer());
    std::vector<vkt::uni_ptr<vlr::SetBase_T<uint8_t>>> sets = {};
    for (uint32_t i = 0; i < model.buffers.size(); i++) {
        auto buffer = std::make_shared<vlr::SetBase_T<uint8_t>>(fw, vlr::DataSetCreateInfo{ .count = model.buffers[i].data.size() }); sets.push_back(buffer);
        memcpy(&buffer->get(0u), model.buffers[i].data.data(), model.buffers[i].data.size());
        //buffers->pushBufferView(buffer->getGpuBuffer());
    };

    // bindings
    for (uint32_t i = 0; i < model.bufferViews.size(); i++) {
        auto bview = model.bufferViews[i];
        auto buffer = sets[bview.buffer];

        // 
        buffers->pushBufferView(vkt::Vector<uint8_t>(buffer->getGpuBuffer().getAllocation(), buffer->getGpuBuffer().offset()+bview.byteOffset, bview.byteLength, bview.byteStride));
        bindings->get(i) = vkh::VkVertexInputBindingDescription{ .binding = i, .stride = uint32_t(bview.byteStride) };
    };

    /*
    //buffers->pushBufferView(vertexData->getGpuBuffer());
    std::vector<vkt::uni_ptr<vlr::SetBase_T<uint8_t>>> sets = {};
    for (uint32_t i = 0; i < model.bufferViews.size(); i++) {
        auto bview = model.bufferViews[i];
        auto buffer = std::make_shared<vlr::SetBase_T<uint8_t>>(fw, vlr::DataSetCreateInfo{ .count = bview.byteLength }); sets.push_back(buffer);
        buffers->pushBufferView(buffer->getGpuBuffer());
        bindings->get(i) = vkh::VkVertexInputBindingDescription{ .binding = i, .stride = uint32_t(bview.byteStride) };
        memcpy(&buffer->get(0u), model.buffers[bview.buffer].data.data() + bview.byteOffset, bview.byteLength);
    };*/

    // accessors
    for (uint32_t i = 0; i < model.accessors.size(); i++) {
        auto accessor = model.accessors[i];
        auto type = VK_FORMAT_R32G32B32_SFLOAT;
        if (accessor.type == TINYGLTF_TYPE_VEC4) type = VK_FORMAT_R32G32B32A32_SFLOAT;
        if (accessor.type == TINYGLTF_TYPE_VEC3) type = VK_FORMAT_R32G32B32_SFLOAT;
        if (accessor.type == TINYGLTF_TYPE_VEC2) type = VK_FORMAT_R32G32_SFLOAT;
        if (accessor.type == TINYGLTF_TYPE_SCALAR) type = VK_FORMAT_R32_SFLOAT;
        accessors->get(i) = vkh::VkVertexInputAttributeDescription{ .binding = uint32_t(accessor.bufferView), .format = type, .offset = uint32_t(accessor.byteOffset) }; // Location NOT supported...
    };

    // 
    auto vertexSet = std::make_shared<vlr::VertexSet>(fw, vlr::VertexSetCreateInfo{
        .bindings = bindings,
        .attributes = accessors,
        .bufferViews = buffers
    });


    //
    std::vector<vkt::uni_ptr<vlr::GeometrySet>> geometrySets = {};
    std::vector<vkt::uni_ptr<vlr::Acceleration>> accelerations = {};

    // 
    for (uint32_t j = 0; j < model.meshes.size(); j++) {
        const auto& meshData = model.meshes[j];

        // 
        std::vector<int64_t> primitiveCountPer = {};
        int64_t vertexCountAll = 0u; bool ctype = true;//false;
        for (uint32_t v = 0; v < meshData.primitives.size(); v++) {
            const auto& primitive = meshData.primitives[v];
            int64_t vertexCount = 0u;
            if (primitive.indices >= 0) {
                vertexCount = model.accessors[primitive.indices].count;
                if (model.accessors[primitive.indices].componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) { ctype = true; };
            } else
            if (primitive.attributes.find("POSITION") != primitive.attributes.end()) { // Vertices
                vertexCount = model.accessors[primitive.attributes.find("POSITION")->second].count;
            };
            vertexCountAll += vertexCount;
            primitiveCountPer.push_back(vkt::tiled(uint64_t(vertexCount), uint64_t(3ull)));
        };

        // 
        const VkDeviceSize PrimitiveCount = std::max(vkt::tiled(uint64_t(vertexCountAll), uint64_t(3ull)), uint64_t(1ull));
        auto geometrySet = std::make_shared<vlr::GeometrySet>(vertexSet, vlr::DataSetCreateInfo{ .count = primitiveCountPer.size() });
        auto acceleration = std::make_shared<vlr::Acceleration>(fw, vlr::AccelerationCreateInfo{ .geometrySet = geometrySet, .initials = primitiveCountPer });

        // 
        for (uint32_t v = 0; v < meshData.primitives.size(); v++) {
            auto geometryDesc = vlr::GeometryDesc{
                .primitiveCount = uint32_t(primitiveCountPer[v]),
                .vertexAttribute = 0u
            };

            const auto& primitive = meshData.primitives[v];
            std::array<std::string, 4u> NM = { "POSITION" , "TEXCOORD_0" , "NORMAL" , "TANGENT" };
            for (uint32_t i = 0u; i < NM.size(); i++) {
                if (primitive.attributes.find(NM[i]) != primitive.attributes.end()) { // Vertices
                    auto& attribute = model.accessors[primitive.attributes.find(NM[i])->second];
                    auto  stride = attribute.ByteStride(model.bufferViews[attribute.bufferView]);

                    // 
                    uint32_t location = 0u;
                    if (NM[i] == "POSITION") { location = 0u; };
                    if (NM[i] == "TEXCOORD_0") { location = 1u; geometryDesc.mesh_flags.hasTexcoord = 1; };
                    if (NM[i] == "NORMAL") { location = 2u; geometryDesc.mesh_flags.hasNormal = 1; };
                    if (NM[i] == "TANGENT") { location = 3u; geometryDesc.mesh_flags.hasTangent = 1; };

                    // 
                    auto type = VK_FORMAT_R32G32B32_SFLOAT;
                    if (attribute.type == TINYGLTF_TYPE_VEC4) type = VK_FORMAT_R32G32B32A32_SFLOAT;
                    if (attribute.type == TINYGLTF_TYPE_VEC3) type = VK_FORMAT_R32G32B32_SFLOAT;
                    if (attribute.type == TINYGLTF_TYPE_VEC2) type = VK_FORMAT_R32G32_SFLOAT;
                    if (attribute.type == TINYGLTF_TYPE_SCALAR) type = VK_FORMAT_R32_SFLOAT;

                    // 
                    if (location == 0u) {
                        geometryDesc.vertexAttribute = primitive.attributes.find(NM[i])->second;
                    } else {
                        geometryDesc.attributes[location - 1u] = primitive.attributes.find(NM[i])->second;
                    };
                };
            };

            //
            geometryDesc.material = primitive.material;
            geometryDesc.primitiveCount = primitiveCountPer[v];

            // 
            if (primitive.indices >= 0) { // determine index type
                auto& attribute = model.accessors[primitive.indices];
                const auto& BV = model.bufferViews[attribute.bufferView];
                const auto range = vkt::tiled(uint64_t(BV.byteLength), uint64_t(4ull)) * uint64_t(4ull);

                // 
                geometryDesc.indexBufferView = attribute.bufferView;
                geometryDesc.indexType = attribute.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT ? VK_INDEX_TYPE_UINT16 : (attribute.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE ? VK_INDEX_TYPE_UINT8_EXT : VK_INDEX_TYPE_UINT32);
            };

            // 
            auto geometry = std::make_shared<vlr::Geometry>(vertexSet, geometryDesc);
            geometrySet->pushGeometry(geometry);
        };

        // 
        accelerations.push_back(acceleration);
        geometrySets.push_back(geometrySet);
    };

    // 
    fw->submitOnce([&](VkCommandBuffer cmd) {
        for (uint32_t i = 0; i < sets.size(); i++) {
            sets[i]->setCommand(cmd);
        };

        // 
        bindings->setCommand(cmd);
        accessors->setCommand(cmd);
        
        // 
        for (uint32_t i = 0; i < geometrySets.size(); i++) {
            geometrySets[i]->setCommand(cmd);
        };
    });

    // Count all Meshes in Scene
    uint32_t instanceCounter = 0u;
    std::shared_ptr<std::function<void(const tinygltf::Node&, glm::dmat4, int)>> vertexCounter = {};
    vertexCounter = std::make_shared<std::function<void(const tinygltf::Node&, glm::dmat4, int)>>([&](const tinygltf::Node& gnode, glm::dmat4 inTransform, int recursive)->void {
        instanceCounter++;
        if (gnode.children.size() > 0 && gnode.mesh < 0) {
            for (int n = 0; n < gnode.children.size(); n++) {
                if (recursive >= 0) (*vertexCounter)(model.nodes[gnode.children[n]], glm::dmat4(inTransform), recursive - 1);
            };
        };
    });

    // Counting itself...
    uint32_t sceneID = 0;
    if (model.scenes.size() > 0) {
        instanceCounter = 0u;
        for (int n = 0; n < model.scenes[sceneID].nodes.size(); n++) {
            auto& gnode = model.nodes[model.scenes[sceneID].nodes[n]];
            (*vertexCounter)(gnode, glm::dmat4(glm::translate(glm::dvec3(0., unitHeight, 0.)) * glm::scale(glm::dvec3(unitScale))), 16);
        };
    };

    // Set by counted meshes
    auto instanceSet = std::make_shared<vlr::InstanceSet>(fw, vlr::DataSetCreateInfo{ .count = instanceCounter });
    auto accelerationTop = std::make_shared<vlr::Acceleration>(fw, vlr::AccelerationCreateInfo{ .instanceSet = instanceSet, .initials = { instanceCounter } });

    //
    instanceSet->setAccelerations(accelerations);

    // Set Instance Filler
    std::shared_ptr<std::function<void(const tinygltf::Node&, glm::dmat4, int)>> vertexLoader = {};
    vertexLoader = std::make_shared<std::function<void(const tinygltf::Node&, glm::dmat4, int)>>([&](const tinygltf::Node& gnode, glm::dmat4 inTransform, int recursive)->void {
        auto localTransform = glm::dmat4(1.0);
        localTransform *= glm::dmat4(gnode.matrix.size() >= 16 ? glm::make_mat4(gnode.matrix.data()) : glm::dmat4(1.0));
        localTransform *= glm::dmat4(gnode.translation.size() >= 3 ? glm::translate(glm::make_vec3(gnode.translation.data())) : glm::dmat4(1.0));
        localTransform *= glm::dmat4(gnode.scale.size() >= 3 ? glm::scale(glm::make_vec3(gnode.scale.data())) : glm::dmat4(1.0));
        localTransform *= glm::dmat4((gnode.rotation.size() >= 4 ? glm::mat4_cast(glm::make_quat(gnode.rotation.data())) : glm::dmat4(1.0)));

        // 
        if (gnode.mesh >= 0) {
            instanceSet->get(instanceCounter++) = vkh::VsGeometryInstance{
                .transform = glm::mat3x4(glm::transpose(glm::dmat4(inTransform) * glm::dmat4(localTransform))),
                .customId = uint32_t(gnode.mesh), // MeshID
                .mask = 0xff,
                .instanceOffset = 0u,
            };
        };

        // 
        if (gnode.children.size() > 0 && gnode.mesh < 0) {
            for (int n = 0; n < gnode.children.size(); n++) {
                if (recursive >= 0) (*vertexLoader)(model.nodes[gnode.children[n]], glm::dmat4(inTransform) * glm::dmat4(localTransform), recursive - 1);
            };
        };
    });

    // Finally, load scene
    if (model.scenes.size() > 0) {
        instanceCounter = 0u;
        for (int n = 0; n < model.scenes[sceneID].nodes.size(); n++) {
            auto& gnode = model.nodes[model.scenes[sceneID].nodes[n]];
            (*vertexLoader)(gnode, glm::dmat4(glm::translate(glm::dvec3(0., unitHeight, 0.)) * glm::scale(glm::dvec3(unitScale))), 16);
        };
    };

    // 
    auto framebuffer = std::make_shared<vlr::Framebuffer>(fw);
    auto layout = std::make_shared<vlr::PipelineLayout>(fw);
    framebuffer->createFramebuffer(canvasWidth, canvasHeight);

    // 
    auto rasterization = std::make_shared<vlr::Rasterization>(fw, vlr::PipelineCreateInfo{
        .layout = layout,
        .framebuffer = framebuffer,
        .instanceSet = instanceSet,
        .constants = constants,
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
    auto materialSet = std::make_shared<vlr::MaterialSet>(fw, vlr::DataSetCreateInfo{ .count = model.materials.size() });
    auto textureSet = std::make_shared<vlr::TextureSet>(fw);
    auto samplerSet = std::make_shared<vlr::SamplerSet>(fw);
    auto background = std::make_shared<vlr::Background>(fw);

    // Material 
    for (uint32_t i = 0; i < model.materials.size(); i++) {
        const auto& mat = model.materials[i]; vlr::MaterialUnit& mdk = materialSet->get(i);
        mdk = vlr::MaterialUnit{};
        mdk.diffuseTexture = mat.pbrMetallicRoughness.baseColorTexture.index;
        mdk.normalTexture = mat.normalTexture.index;
        mdk.pbrAGMTexture = mat.pbrMetallicRoughness.metallicRoughnessTexture.index;
        mdk.emissionTexture = mat.emissiveTexture.index;

        // 
        mdk.normal = glm::vec4(0.5f, 0.5f, 1.0f, 1.f);
        mdk.emission = glm::vec4(0.f);
        mdk.diffuse = glm::vec4(1.f);
        mdk.pbrAGM = glm::vec4(1.f, mat.pbrMetallicRoughness.roughnessFactor, mat.pbrMetallicRoughness.metallicFactor, 0.f);

        // 
        if (mat.emissiveFactor.size() > 0) {
            mdk.emission = glm::vec4(mat.emissiveFactor[0], mat.emissiveFactor[1], mat.emissiveFactor[2], 0.f);
        };

        // 
        if (mat.pbrMetallicRoughness.baseColorFactor.size() > 0) {
            mdk.diffuse = glm::vec4(mat.pbrMetallicRoughness.baseColorFactor[0], mat.pbrMetallicRoughness.baseColorFactor[1], mat.pbrMetallicRoughness.baseColorFactor[2], 1.f);
        };

        // 
        materialSet->get(i) = mdk;
    };

    // TODO: GLTF Samplers Support for shaders
    for (uint32_t i = 0; i < model.samplers.size(); i++) {
        const auto& smp = model.samplers[i];

        // 
        VkSampler sampler = {};
        vkh::handleVk(fw->getDeviceDispatch()->CreateSampler(vkh::VkSamplerCreateInfo{
            .magFilter = VK_FILTER_LINEAR,
            .minFilter = VK_FILTER_LINEAR,
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        }, nullptr, &sampler));

        // 
        samplerSet->pushSampler(sampler);
    };

    // 
    if (model.samplers.size() <= 0u) {
        VkSampler sampler = {};
        vkh::handleVk(fw->getDeviceDispatch()->CreateSampler(vkh::VkSamplerCreateInfo{
            .magFilter = VK_FILTER_LINEAR,
            .minFilter = VK_FILTER_LINEAR,
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        }, nullptr, & sampler));
        samplerSet->pushSampler(sampler);
    };

    // 
    for (uint32_t i = 0; i < model.images.size(); i++) {
        const auto& img = model.images[i];

        //
        vkh::VkImageCreateFlags flg = {};
        vkt::unlock32(flg) = 0u;

        // 
        auto image = vkt::ImageRegion(std::make_shared<vkt::VmaImageAllocation>(fw->getAllocator(), vkh::VkImageCreateInfo{  // experimental: callify
            .flags = flg, .format = VK_FORMAT_R8G8B8A8_UNORM, .extent = {uint32_t(img.width),uint32_t(img.height),1u}, .usage = imageUsage,
        }, vkt::VmaMemoryInfo{ .memUsage = VMA_MEMORY_USAGE_GPU_ONLY }), vkh::VkImageViewCreateInfo{
            .format = VK_FORMAT_R8G8B8A8_UNORM,
            .subresourceRange = apres
        });

        //
        vkt::Vector<> imageBuf = {};
        if (img.image.size() > 0u) {
            imageBuf = vkt::Vector<>(std::make_shared<vkt::VmaBufferAllocation>(fw->getAllocator(), vkh::VkBufferCreateInfo{ // experimental: callify
                .size = img.image.size(), .usage = uploadUsage,
            }, vkt::VmaMemoryInfo{ .memUsage = VMA_MEMORY_USAGE_CPU_TO_GPU }));
            memcpy(imageBuf.data(), &img.image[0u], img.image.size());
        };

        // 
        fw->submitOnce([&](VkCommandBuffer& cmd) { image.transfer(cmd);
            auto buffer = imageBuf.has() ? imageBuf : buffers->get(img.bufferView);
            fw->getDeviceDispatch()->CmdCopyBufferToImage(cmd, buffer.buffer(), image.getImage(), image.getImageLayout(), 1u, vkh::VkBufferImageCopy{
                .bufferOffset = buffer.offset(),
                .bufferRowLength = uint32_t(img.width),
                .bufferImageHeight = uint32_t(img.height),
                .imageSubresource = image.subresourceLayers(),
                .imageOffset = {0u,0u,0u},
                .imageExtent = {uint32_t(img.width),uint32_t(img.height),1u},
            });
        });

        textureSet->pushImage(image);
    };

    // 
    if (model.images.size() <= 0u) {
        // 
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


    // 
    layout->setFramebuffer(framebuffer);
    layout->setBackground(background);
    layout->setMaterials(materialSet, textureSet, samplerSet);
    layout->setVertexSet(vertexSet);

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
    pipelineInfo.graphicsPipelineCreateInfo.renderPass = renderPass;//context->refRenderPass();
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
    auto rtCommand = vkt::createCommandBuffer(fw->getDeviceDispatch(), commandPool, false, false);
    {
        buildCommand->setCommand(rtCommand);
        instanceSet->setCommand(rtCommand, true);
        buildCommand->setCommandTop(rtCommand); // NEW! 05.08.2020

        // 
        materialSet->setCommand(rtCommand);
        constants->setCommand(rtCommand, true);
        renderCommand->setCommand(rtCommand);
        rayTracing->setCommandFinal(rtCommand);

        // 
        vkt::commandBarrier(fw->getDeviceDispatch(), rtCommand);
        fw->getDeviceDispatch()->EndCommandBuffer(rtCommand);
    };

    // 
    while (!glfwWindowShouldClose(manager.window)) { // 
        glfwPollEvents();

        // 
        //if (rdoc_api) rdoc_api->StartFrameCapture(NULL, NULL);

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
            glm::mat4 tran = cameraController->translation();

            // 
            constants->get(0u).modelview = glm::transpose(glm::mat4x3(proj * tran));
            constants->get(0u).projection = glm::transpose(glm::mat4x4(glm::perspective(80.f / 180.f * glm::pi<double>(), double(canvasWidth) / double(canvasHeight), 0.001, 10000.)));
            constants->get(0u).modelviewInv = glm::transpose(glm::mat4x3(glm::inverse(proj * tran)));
            constants->get(0u).projectionInv = glm::transpose(glm::mat4x4(glm::inverse(glm::perspective(80.f / 180.f * glm::pi<double>(), double(canvasWidth) / double(canvasHeight), 0.001, 10000.))));

            // Create render submission 
            std::vector<VkSemaphore> waitSemaphores = { framebuffers[currentBuffer].presentSemaphore }, signalSemaphores = { framebuffers[currentBuffer].computeSemaphore };
            std::vector<vkh::VkPipelineStageFlags> waitStages = {
                vkh::VkPipelineStageFlags{.eFragmentShader = 1, .eComputeShader = 1, .eTransfer = 1, .eRayTracingShader = 1, .eAccelerationStructureBuild = 1 },
                vkh::VkPipelineStageFlags{.eFragmentShader = 1, .eComputeShader = 1, .eTransfer = 1, .eRayTracingShader = 1, .eAccelerationStructureBuild = 1 }
            };

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
                fw->getDeviceDispatch()->CmdBeginRenderPass(commandBuffer, vkh::VkRenderPassBeginInfo{ .renderPass = renderPass, .framebuffer = framebuffers[currentBuffer].frameBuffer, .renderArea = renderArea, .clearValueCount = 2u, .pClearValues = reinterpret_cast<vkh::VkClearValue*>(&clearValues[0]) }, VK_SUBPASS_CONTENTS_INLINE);
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
        //if (rdoc_api) rdoc_api->EndFrameCapture(NULL, NULL);
    };

    // 
    glfwDestroyWindow(manager.window);
    glfwTerminate(); exit(0);
    return 0;
};
