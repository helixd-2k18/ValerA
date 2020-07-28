
#define VKT_ENABLE_GLFW_SUPPORT
#define GLM_FORCE_SWIZZLE 
#define GLM_SWIZZLE_XYZW 
#define GLM_SWIZZLE_STQP

// 
#include "vlr/Config.hpp"
#include "vlr/Driver.hpp"

// #
#if defined(_WIN32) && !defined(WIN32)
#define WIN32
#endif

#ifdef WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif

//
//#define VMA_IMPLEMENTATION
//#define TINYGLTF_IMPLEMENTATION
//#define TINYEXR_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION

// 
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <misc/tiny_gltf.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

//
namespace vkx {

};

// 
struct Active {
    std::vector<uint8_t> keys = {};
    std::vector<uint8_t> mouse = {};

    double mX = 1e-5, mY = 1e-5, dX = 0.0, dY = 0.0;
    double tDiff = 0.0, tCurrent = 1e-5;
};

// 
class Shared : public std::enable_shared_from_this<Shared> {
public:
    static Active active; // shared properties
    static GLFWwindow* window; // in-bound GLFW window
    //friend Renderer;

    static void TimeCallback(double milliseconds = 1e-5) {
        Shared::active.tDiff = milliseconds - Shared::active.tCurrent, Shared::active.tCurrent = milliseconds;
        Shared::active.dX = 0.0, Shared::active.dY = 0.0;
    };

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) Shared::active.keys[key] = uint8_t(1u);
        if (action == GLFW_RELEASE) Shared::active.keys[key] = uint8_t(0u);
        if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) { glfwTerminate(); exit(0); };
    };

    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        if (action == GLFW_PRESS) Shared::active.mouse[button] = uint8_t(1u);
        if (action == GLFW_RELEASE) Shared::active.mouse[button] = uint8_t(0u);
    };

    static void MouseMoveCallback(GLFWwindow* window, double xpos = 1e-5, double ypos = 1e-5) {
        Shared::active.dX = xpos - Shared::active.mX, Shared::active.dY = ypos - Shared::active.mY; // get diff with previous position
        Shared::active.mX = xpos, Shared::active.mY = ypos; // set current mouse position
    };
};


// 
Active Shared::active; // shared properties
GLFWwindow* Shared::window; // in-bound GLFW window


// camera binding control 
class CameraController : public std::enable_shared_from_this<CameraController> {
public:
    // use pointers
    glm::dvec3* viewVector = nullptr;
    glm::dvec3* eyePos = nullptr;
    glm::dvec3* upVector = nullptr;
    glm::uvec2* canvasSize = nullptr;

    // create relative control matrice
    auto project() { return glm::lookAt(*eyePos, (*eyePos + *viewVector), *upVector); };

    // event handler
    CameraController& handle() {
        //auto mPtr = (glm::dvec2*)&Shared::active.mX, mDiff = *mPtr - mousePosition;
        auto mDiff = glm::dvec2(Shared::active.dX, Shared::active.dY);
        auto tDiff = Shared::active.tDiff;

        glm::dmat4 viewm = this->project();
        glm::dmat4 unviewm = glm::inverse(viewm);
        glm::dvec3 ca = (viewm * glm::dvec4(*eyePos, 1.0)).xyz(), vi = glm::normalize((glm::dvec4(*viewVector, 0.0) * unviewm)).xyz();
        bool isFocus = true;

        if (Shared::active.mouse.size() > 0 && Shared::active.mouse[GLFW_MOUSE_BUTTON_LEFT] && isFocus)
        {
            if (glm::abs(mDiff.x) > 0.0) this->rotateX(vi, mDiff.x);
            if (glm::abs(mDiff.y) > 0.0) this->rotateY(vi, mDiff.y);
        }

        if (Shared::active.keys.size() > 0 && Shared::active.keys[GLFW_KEY_W] && isFocus)
        {
            this->forwardBackward(ca, -tDiff);
        }

        if (Shared::active.keys.size() > 0 && Shared::active.keys[GLFW_KEY_S] && isFocus)
        {
            this->forwardBackward(ca, tDiff);
        }

        if (Shared::active.keys.size() > 0 && Shared::active.keys[GLFW_KEY_A] && isFocus)
        {
            this->leftRight(ca, -tDiff);
        }

        if (Shared::active.keys.size() > 0 && Shared::active.keys[GLFW_KEY_D] && isFocus)
        {
            this->leftRight(ca, tDiff);
        }

        if (Shared::active.keys.size() > 0 && (Shared::active.keys[GLFW_KEY_SPACE] || Shared::active.keys[GLFW_KEY_E]) && isFocus)
        {
            this->topBottom(ca, tDiff);
        }

        if (Shared::active.keys.size() > 0 && (Shared::active.keys[GLFW_KEY_LEFT_SHIFT] || Shared::active.keys[GLFW_KEY_C] || Shared::active.keys[GLFW_KEY_Q]) && isFocus)
        {
            this->topBottom(ca, -tDiff);
        }

        *viewVector = glm::normalize((glm::dvec4(vi, 0.0) * viewm).xyz());
        *eyePos = (unviewm * glm::dvec4(ca, 1.0)).xyz();

        return *this;
    }


    // sub-contollers
    CameraController& leftRight(glm::dvec3& ca, const double& diff) {
        ca.x += diff / 100.0;
        return *this;
    }

    CameraController& topBottom(glm::dvec3& ca, const double& diff) {
        ca.y += diff / 100.0;
        return *this;
    }

    CameraController& forwardBackward(glm::dvec3& ca, const double& diff) {
        ca.z += diff / 100.0;
        return *this;
    }

    CameraController& rotateY(glm::dvec3& vi, const double& diff) {
        glm::dmat4 rot = glm::rotate(diff / double(canvasSize->y) * 2., glm::dvec3(-1.0, 0.0, 0.0));
        vi = (rot * glm::vec4(vi, 1.0)).xyz();
        return *this;
    }

    CameraController& rotateX(glm::dvec3& vi, const double& diff) {
        glm::dmat4 rot = glm::rotate(diff / double(canvasSize->x) * 2., glm::dvec3(0.0, -1.0, 0.0));
        vi = (rot * glm::vec4(vi, 1.0)).xyz();
        return *this;
    }
};


#if defined(ENABLE_OPENGL_INTEROP) && !defined(VKT_USE_GLAD)
using namespace gl;
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
#ifdef ENABLE_OPENGL_INTEROP
    glViewport(0, 0, width, height);
#endif
}

std::shared_ptr<vkt::GPUFramework> fw = {};

// FOR FUTURE USAGE!
class RayTracer : public std::enable_shared_from_this<RayTracer> {
public:
    ~RayTracer() {};
    RayTracer() {
        this->fw = std::make_shared<vkt::GPUFramework>();
    };

    // 
    std::shared_ptr<vkt::GPUFramework> fw = {};
};

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

struct FDStruct {
    glm::vec4 fPosition = glm::vec4(1.f);
    glm::vec4 fTexcoord = glm::vec4(1.f);
    glm::vec4 fNormal = glm::vec4(1.f);
    glm::vec4 fTangent = glm::vec4(1.f);
    glm::vec4 fBinormal = glm::vec4(1.f);
};

void error(int errnum, const char* errmsg)
{
    std::cerr << errnum << ": " << errmsg << std::endl;
};

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
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);

    // TODO: SPECIFIC GL_API
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // OpenGL IS NOT MAIN! IT FOR TEST ONLY!
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // For OpenGL Context
    auto& appObj = fw->getAppObject();

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
    }
#endif

    // 
    float xscale = 1.f, yscale = 1.f;
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    glfwGetMonitorContentScale(primary, &xscale, &yscale);

    // 
    auto renderArea = vkh::VkRect2D{ vkh::VkOffset2D{0, 0}, vkh::VkExtent2D{ uint32_t(canvasWidth / 2.f * xscale), uint32_t(canvasHeight / 2.f * yscale) } };
    auto viewport = vkh::VkViewport{ 0.0f, 0.0f, static_cast<float>(renderArea.extent.width), static_cast<float>(renderArea.extent.height), 0.f, 1.f };

    // 
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








    return 0u;
};
