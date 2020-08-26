#include "../include/main.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>
#include <glm/gtx/matrix_decompose.hpp>

//#define STB_IMAGE_IMPLEMENTATION
//#include <misc/stb_image.h>
#include <filesystem>


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
    auto constants = std::make_shared<vlr::Constants>(fw, vlr::DataSetCreateInfo{ .count = 1u, .uniform = true });

    //
    std::string inputfile = "lost_empire.obj";
    tinyobj::attrib_t attrib = {};
    std::vector<tinyobj::shape_t> shapes = {};
    std::vector<tinyobj::material_t> materials = {};

    //
    std::string warn = "";
    std::string err = "";

    //
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());
    if (!warn.empty()) { std::cout << warn << std::endl; }
    if (!err.empty()) { std::cerr << err << std::endl; }
    if (!ret) { exit(1); }

    // For Debug! 
    //shapes.push_back(shapes[0]);

    // Loop over shapes
    VkDeviceSize accessorCount = 0u;
    std::vector<std::vector<VkDeviceSize>> primitiveCountPer = {};
    std::vector<VkDeviceSize> vertexCountAll = {};
    for (size_t s = 0; s < shapes.size(); s++) {
        vertexCountAll.push_back(0ull);
        primitiveCountPer.push_back({});
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) { //
            primitiveCountPer.back().push_back(shapes[s].mesh.num_face_vertices[f]);
            vertexCountAll.back() += shapes[s].mesh.num_face_vertices[f];
            accessorCount++;
        };
    };

    // 
    accessorCount = 0u;
    std::vector<vkt::uni_ptr<vlr::SetBase_T<FDStruct>>> sets = {};
    std::vector<vkt::uni_ptr<vlr::Acceleration>> accelerations = {};
    std::vector<vkt::uni_ptr<vlr::GeometrySet>> geometries = {};

    // 
    auto denoiser = std::make_shared<vlr::OptiXDenoise>(fw);
    auto bindings = std::make_shared<vlr::BindingSet>(fw, vlr::DataSetCreateInfo{ .count = shapes.size() * 2u });
    auto accessors = std::make_shared<vlr::AttributeSet>(fw, vlr::DataSetCreateInfo{ .count = shapes.size() * 3u });
    auto buffers = std::make_shared<vlr::BufferViewSet>(fw);

    // 
    auto vertexSet = std::make_shared<vlr::VertexSet>(fw, vlr::VertexSetCreateInfo{
        .bindings = bindings,
        .attributes = accessors,
        .bufferViews = buffers
    });

    // 
    auto instanceSet = std::make_shared<vlr::InstanceSet>(fw, vlr::DataSetCreateInfo{ .count = shapes.size() });
    auto accelerationTop = std::make_shared<vlr::Acceleration>(fw, vlr::AccelerationCreateInfo{ .instanceSet = instanceSet, .initials = {shapes.size()} }); // shapes.size()

    // 
    for (size_t s = 0; s < shapes.size(); s++) { // 
        auto vertexData = std::make_shared<vlr::SetBase_T<FDStruct>>(fw, vlr::DataSetCreateInfo{ .count = vertexCountAll[s] });
        auto geometrySet = std::make_shared<vlr::GeometrySet>(vertexSet, vlr::DataSetCreateInfo{ .count = shapes.size() });
        auto acceleration = std::make_shared<vlr::Acceleration>(fw, vlr::AccelerationCreateInfo{ .geometrySet = geometrySet, .initials = primitiveCountPer[s] });

        // 
        //if (s == 0u) {
            sets.push_back(vertexData);
        //} else {
            //sets.push_back(sets[0]);
        //};

        accelerations.push_back(acceleration);
        geometries.push_back(geometrySet);
        buffers->pushBufferView(sets.back()->getGpuBuffer());

        // 
        size_t indexOffset = 0; // Loop over faces(polygon)

        // 
        auto gdesc = vlr::GeometryDesc{
            .firstVertex = 0u,//indexOffset,
            .primitiveCount = uint32_t(indexOffset / 3u),
            .material = uint32_t(shapes[s].mesh.material_ids[0u] != -1 ? shapes[s].mesh.material_ids[0u] : 0u),
            .vertexAttribute = 0u
        };

        //
        gdesc.mesh_flags.translucent = 1;

        // 
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) { //

            // 
            auto verticeCount = shapes[s].mesh.num_face_vertices[f];
            for (size_t v = 0; v < verticeCount; v++) { //
                tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];
                vertexData->get(indexOffset + v).fPosition = glm::vec4(attrib.vertices[3 * idx.vertex_index + 0], attrib.vertices[3 * idx.vertex_index + 1], attrib.vertices[3 * idx.vertex_index + 2], 1.f);
                if (idx.normal_index >= 0) { 
                    vertexData->get(indexOffset + v).fNormal = glm::vec4(attrib.normals[3 * idx.normal_index + 0], attrib.normals[3 * idx.normal_index + 1], attrib.normals[3 * idx.normal_index + 2], 0.f); 
                    gdesc.mesh_flags.hasNormal = 1;
                };
                if (idx.texcoord_index >= 0) {
                    vertexData->get(indexOffset + v).fTexcoord = glm::vec4(attrib.texcoords[2 * idx.texcoord_index + 0], attrib.texcoords[2 * idx.texcoord_index + 1], 0.f, 0.f);
                    gdesc.mesh_flags.hasTexcoord = 1;
                };
            };

            indexOffset += verticeCount;
        };

        // 
        gdesc.primitiveCount = uint32_t(indexOffset / 3u);

        // 
        uint32_t bID = uint32_t(s);
        bindings->get(bID) = vkh::VkVertexInputBindingDescription{
            .binding = bID, .stride = sizeof(FDStruct)
        };

        // 
        //accessorCount = 0u; // for DEBUG

        // 
        accessors->get(gdesc.vertexAttribute = accessorCount++) = vkh::VkVertexInputAttributeDescription{
            .location = 0u, .binding = bID,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = uint32_t(offsetof(FDStruct, fPosition))
        };

        // 
        accessors->get(gdesc.attributes[0u] = accessorCount++) = vkh::VkVertexInputAttributeDescription{
            .location = 0u, .binding = bID,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = uint32_t(offsetof(FDStruct, fTexcoord))
        };

        // 
        accessors->get(gdesc.attributes[1u] = accessorCount++) = vkh::VkVertexInputAttributeDescription{
            .location = 0u, .binding = bID,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = uint32_t(offsetof(FDStruct, fNormal))
        };

        // 
        auto geometry = std::make_shared<vlr::Geometry>(vertexSet, gdesc);

        // 
        geometrySet->pushGeometry(geometry);
        instanceSet->pushAcceleration(acceleration);

        // 
        auto geometryData = geometrySet->getVector();
    };

    // 
    auto accessorsV = accessors->getVector();
    auto bindingsV = bindings->getVector();

    // 
    fw->submitOnce([&](VkCommandBuffer cmd) {
        bindings->setCommand(cmd);
        accessors->setCommand(cmd);
        for (size_t s = 0; s < shapes.size(); s++) {
            sets[s]->setCommand(cmd);
            geometries[s]->setCommand(cmd);
        };
    });

    // 
    auto framebuffer = std::make_shared<vlr::Framebuffer>(fw);
    auto layout = std::make_shared<vlr::PipelineLayout>(fw);

    // 
    for (size_t s = 0; s < shapes.size(); s++) {
        instanceSet->get(s) = vkh::VsGeometryInstance{
            .customId = uint32_t(s),
            .accelerationStructureHandle = accelerations[s]->getHandle() // Broken Feature, Set Manually
        };
    };

    // 
    auto rasterization = std::make_shared<vlr::Rasterization>(fw, vlr::PipelineCreateInfo{
        .layout = layout,
        .framebuffer = framebuffer,
        .instanceSet = instanceSet,
        .constants = constants
    });

    // 
    auto rayTracing = std::make_shared<vlr::RayTracing>(fw, vlr::RayTracingCreateInfo{
        .layout = layout,
        .framebuffer = framebuffer,
        .accelerationTop = accelerationTop,
        .constants = constants
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
        .accelerationTop = accelerationTop
    });

    

    //  
    auto materialSet = std::make_shared<vlr::MaterialSet>(fw, vlr::DataSetCreateInfo{ .count = std::max(uint32_t(materials.size()), 1u) });
    auto textureSet = std::make_shared<vlr::TextureSet>(fw);
    auto samplerSet = std::make_shared<vlr::SamplerSet>(fw);
    auto background = std::make_shared<vlr::Background>(fw);


    // 
    std::string base_dir = "./";
    std::unordered_map<std::string, uint32_t> textures = {};
    uint32_t idx = 0u;

    // 
    auto loadTexture = [&](std::string name) -> GLuint {
        // Only load the texture if it is not already loaded
        if (textures.find(name) == textures.end()) {
            int w = 0u, h = 0u, comp = 4u;

            // 
            std::string texture_filename = name;
            if (!std::filesystem::exists(texture_filename)) {
                // Append base dir.
                texture_filename = base_dir + name;
                if (!std::filesystem::exists(texture_filename)) {
                    std::cerr << "Unable to find file: " << name << std::endl;
                    return -1;
                };
            };

            // 
            stbi_set_flip_vertically_on_load(true);
            unsigned char* image = stbi_load(texture_filename.c_str(), &w, &h, &comp, STBI_rgb_alpha);
            if (!image) {
                std::cerr << "Unable to load texture: " << texture_filename << std::endl;
                return -1;
            };
            std::cout << "Loaded texture: " << texture_filename << ", w = " << w << ", h = " << h << ", comp = " << comp << std::endl;

 
            int width = w, height = h;
            unsigned char* rgba = image;
            const char* err = nullptr;
            GLuint texture_id = idx++;

            {   //
                vkt::VmaMemoryInfo memInfo = {};
                memInfo.memUsage = VMA_MEMORY_USAGE_GPU_ONLY;

                //
                auto image = vkt::ImageRegion(std::make_shared<vkt::VmaImageAllocation>(fw->getAllocator(), vkh::VkImageCreateInfo{}.also([=](vkh::VkImageCreateInfo* it) {
                    it->format = VK_FORMAT_R8G8B8A8_UNORM,
                    it->extent = vkh::VkExtent3D{ uint32_t(width),uint32_t(height),1u },
                    it->usage = imageUsage;
                    return it;
                }), memInfo), vkh::VkImageViewCreateInfo{}.also([=](vkh::VkImageViewCreateInfo* it) {
                    it->format = VK_FORMAT_R8G8B8A8_UNORM,
                    it->subresourceRange = apres;
                    return it;
                }));

                //
                vkt::Vector<> imageBuf = {};

                //if (comp == 4) {
                if (width > 0u && height > 0u && rgba) {
                    memInfo.memUsage = VMA_MEMORY_USAGE_CPU_TO_GPU;
                    imageBuf = vkt::Vector<>(std::make_shared<vkt::VmaBufferAllocation>(fw->getAllocator(), vkh::VkBufferCreateInfo{ // experimental: callify
                        .size = size_t(width) * size_t(height) * sizeof(glm::u8vec4), .usage = uploadUsage,
                    }, memInfo));
                    memcpy(imageBuf.data(), rgba, imageBuf.range());
                };

                // 
                fw->submitOnce([=](VkCommandBuffer& cmd) {
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

            stbi_image_free(image);
            textures.insert(std::make_pair(name, texture_id));
        };

        // If not found, force return -1
        if (textures.find(name) == textures.end()) {
            return -1;
        };

        // Return when possible
        return textures[name];
    };



    // Material
    for (uint32_t i = 0; i < materials.size(); i++) {
        const auto& mat = materials[i]; vlr::MaterialUnit& mdk = materialSet->get(i);
        mdk = vlr::MaterialUnit{};
        mdk.diffuseTexture = -1;
        mdk.normalTexture = -1;
        mdk.pbrAGMTexture = -1;
        mdk.emissionTexture = -1;
        mdk.pbrAGM = glm::vec4(1.f, mat.roughness, mat.metallic, 0.f);
        mdk.normal = glm::vec4(0.5f, 0.5f, 1.0f, 1.f);
        mdk.emission = glm::vec4(mat.emission[0], mat.emission[1], mat.emission[2], 0.f);
        mdk.diffuse = glm::vec4(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1.f);
        mdk.diffuseTexture = loadTexture(mat.diffuse_texname);
        mdk.normalTexture = loadTexture(mat.normal_texname);
        mdk.pbrAGMTexture = loadTexture(mat.specular_texname);
        if (mdk.normalTexture == -1) { mdk.normalTexture = loadTexture(mat.bump_texname); };
    };

    // Default Material
    if (materials.size() <= 0) {
        vlr::MaterialUnit& mdk = materialSet->get(0u);
        mdk = vlr::MaterialUnit{};
        mdk.diffuseTexture = -1;
        mdk.normalTexture = -1;
        mdk.pbrAGMTexture = -1;
        mdk.emissionTexture = -1;
        mdk.diffuse = glm::vec4(1.f, 1.f, 1.f, 1.f);
    };



    // 
    framebuffer->createFramebuffer(canvasWidth, canvasHeight);
    denoiser->setFramebuffer(framebuffer);

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
    Shared::active.mouse.resize(256, uint8_t(0u));
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
            glm::dmat4 proj = cameraController->handle().project();
            glm::dmat4 mv = cameraController->translation();
            //glm::dmat4 mv = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.f, -1.0), glm::vec3(0.0, 1.0, 0.0));

            // 
            glm::dvec3 scale;
            glm::dquat rotation;
            glm::dvec3 translation;
            glm::dvec3 skew;
            glm::dvec4 perspective;
            glm::decompose(proj, scale, rotation, translation, skew, perspective);

            // 
            //glm::dmat4 trans = glm::translate(translation);
            //mv = trans * mv;
            //proj = glm::inverse(trans) * proj;

            // 
            //constants->get(0u) = vlr::ConstantDesc{};
            constants->get(0u).modelview = glm::mat3x4(glm::transpose(proj));
            constants->get(0u).projection = glm::mat4(glm::transpose(glm::perspective(80.0 / 180.0 * glm::pi<double>(), double(canvasWidth) / double(canvasHeight), 0.001, 10000.)));
            constants->get(0u).modelviewInv = glm::mat3x4(glm::transpose(glm::inverse(proj)));
            constants->get(0u).projectionInv = glm::mat4(glm::transpose(glm::inverse(glm::perspective(80.f / 180.f * glm::pi<double>(), double(canvasWidth) / double(canvasHeight), 0.001, 10000.))));

            // Create render submission 
            std::vector<VkSemaphore> waitSemaphores = {}, signalSemaphores = {};
            std::vector<vkh::VkPipelineStageFlags> waitStages = {};

            //
            double scal = glfwGetTime();
            glm::dmat4 trans = glm::scale(glm::sin(scal) * 0.1f + glm::dvec3(1.f, 1.f, 1.f));

            // 
            //mv = mv * glm::scale(glm::f64vec3(100.0));

            // 
            for (size_t s = 0; s < shapes.size(); s++) {
                instanceSet->get(s) = vkh::VsGeometryInstance{
                    .transform = glm::mat3x4(glm::transpose(mv)),
                    .customId = uint32_t(s),
                    .accelerationStructureHandle = accelerations[s]->getHandle() // Broken Feature, Set Manually
                };
            };

            // 
            {   // Path Tracing Command...
                auto rtCommand = vkt::createCommandBuffer(fw->getDeviceDispatch(), commandPool, false, false);

                // 
                buildCommand->setCommand(rtCommand);
                instanceSet->setCommand(rtCommand, true);
                buildCommand->setCommandTop(rtCommand); // NEW! 05.08.2020

                // 
                materialSet->setCommand(rtCommand);
                constants->setCommand(rtCommand, true);
                renderCommand->setCommand(rtCommand);
                vkt::commandBarrier(fw->getDeviceDispatch(), rtCommand);

                // 
                //fw->getDeviceDispatch()->CmdCopyBuffer(rtCommand, counters->getGpuBuffer(), counters->getCpuBuffer(), 1u, vkh::VkBufferCopy{
                //    counters->getGpuBuffer().offset(),
                //    counters->getCpuBuffer().offset(),
                //    counters->getCpuBuffer().range()
                //});

                // 
                fw->getDeviceDispatch()->EndCommandBuffer(rtCommand);

                // 
                VkFence fence = VK_NULL_HANDLE;
                vkh::handleVk(fw->getDeviceDispatch()->CreateFence(vkh::VkFenceCreateInfo{ .flags = {1} }, nullptr, &fence));
                vkh::handleVk(fw->getDeviceDispatch()->QueueSubmit(queue, 1u, vkh::VkSubmitInfo{
                    .waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size()), .pWaitSemaphores = waitSemaphores.data(), .pWaitDstStageMask = waitStages.data(),
                    .commandBufferCount = 1u, .pCommandBuffers = &rtCommand,
                    .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()), .pSignalSemaphores = signalSemaphores.data()
                }, fence));

                // 
                vkh::handleVk(fw->getDeviceDispatch()->WaitForFences(1u, &fence, true, 30ull * 1000ull * 1000ull * 1000ull));
                fw->getDeviceDispatch()->DestroyFence(fence, nullptr);
            };

            // TODO: OptiX Denoise!
            {
                denoiser->denoise(4u); // Denoise Diffuse
                //denoiser->denoise(8u); // Denoise Reflections
            };

            // Next Compute After Denoise
            signalSemaphores = { framebuffers[currentBuffer].computeSemaphore };
            {
                // Path Tracing...
                auto rtCommand = vkt::createCommandBuffer(fw->getDeviceDispatch(), commandPool, false, false);
                rayTracing->setCommandFinal(rtCommand);
                vkt::commandBarrier(fw->getDeviceDispatch(), rtCommand);
                fw->getDeviceDispatch()->EndCommandBuffer(rtCommand);

                // Submit command once
                vkh::handleVk(fw->getDeviceDispatch()->QueueSubmit(queue, 1u, vkh::VkSubmitInfo{
                    .waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size()), .pWaitSemaphores = waitSemaphores.data(), .pWaitDstStageMask = waitStages.data(),
                    .commandBufferCount = 1u, .pCommandBuffers = &rtCommand,
                    .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()), .pSignalSemaphores = signalSemaphores.data()
                }, {}));
            };

            // 
            waitSemaphores = { framebuffers[currentBuffer].presentSemaphore, framebuffers[currentBuffer].computeSemaphore }, signalSemaphores = { framebuffers[currentBuffer].drawSemaphore };
            waitStages = {
                vkh::VkPipelineStageFlags{.eFragmentShader = 1, .eComputeShader = 1, .eTransfer = 1, .eRayTracingShader = 1, .eAccelerationStructureBuild = 1 },
                vkh::VkPipelineStageFlags{.eFragmentShader = 1, .eComputeShader = 1, .eTransfer = 1, .eRayTracingShader = 1, .eAccelerationStructureBuild = 1 }
            };

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
        if (rdoc_api) rdoc_api->EndFrameCapture(NULL, NULL);
    };

    // 
    glfwDestroyWindow(manager.window);
    glfwTerminate(); exit(0);
    return 0;
};
