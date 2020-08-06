#include "./vlr/OptiXDenoise.hpp"
#include "./vlr/Framebuffer.hpp"

#ifdef ENABLE_OPTIX_DENOISE
#include <cuda.h>
#include <cuda_runtime.h>
#include <optix/optix.h>
#include <optix/optix_function_table_definition.h>
#include <optix/optix_stubs.h>
#include <optix/optix_types.h>
#endif

namespace vlr {

#ifdef ENABLE_OPTIX_DENOISE

#define OPTIX_CHECK(call)                                                      \
  do {                                                                         \
    OptixResult res = call;                                                    \
    if (res != OPTIX_SUCCESS) {                                                \
      std::stringstream ss;                                                    \
      ss << "Optix call (" << #call << " ) failed with code " << res           \
         << " (" __FILE__ << ":" << __LINE__ << ")\n";                         \
      std::cerr << ss.str().c_str() << std::endl;                              \
      throw std::runtime_error(ss.str().c_str());                              \
    }                                                                          \
  } while (false)

#define CUDA_CHECK(call)                                                       \
  do {                                                                         \
    cudaError_t error = call;                                                  \
    if (error != cudaSuccess) {                                                \
      std::stringstream ss;                                                    \
      ss << "CUDA call (" << #call << " ) failed with code " << error          \
         << " (" __FILE__ << ":" << __LINE__ << ")\n";                         \
      throw std::runtime_error(ss.str().c_str());                              \
    }                                                                          \
  } while (false)

#define OPTIX_CHECK_LOG(call)                                                  \
  do {                                                                         \
    OptixResult res = call;                                                    \
    if (res != OPTIX_SUCCESS) {                                                \
      std::stringstream ss;                                                    \
      ss << "Optix call (" << #call << " ) failed with code " << res           \
         << " (" __FILE__ << ":" << __LINE__ << ")\nLog:\n"                    \
         << log << "\n";                                                       \
      throw std::runtime_error(ss.str().c_str());                              \
    }                                                                          \
  } while (false)

    OptixDeviceContext m_optixDevice;

    static void context_log_cb(unsigned int level, const char* tag,
        const char* message, void* /*cbdata */) {
        std::cerr << "[" << std::setw(2) << level << "][" << std::setw(12) << tag << "]: " << message << "\n";
    };

    void OptiXDenoise::constructor(vkt::uni_ptr<Driver> driver) {
        // Forces the creation of an implicit CUDA context
        cudaFree(nullptr);

        // 
        CUcontext cuCtx;
        CUresult  cuRes = cuCtxGetCurrent(&cuCtx);
        if (cuRes != CUDA_SUCCESS)
        {
            std::cerr << "Error querying current context: error code " << cuRes << "\n";
        };

        // 
        OPTIX_CHECK(optixInit());
        OPTIX_CHECK(optixDeviceContextCreate(cuCtx, nullptr, &m_optixDevice));
        OPTIX_CHECK(optixDeviceContextSetLogCallback(m_optixDevice, context_log_cb, nullptr, 4));

        // 
        OptixPixelFormat pixelFormat = OPTIX_PIXEL_FORMAT_FLOAT4;
        size_t           sizeofPixel = sizeof(glm::vec4);

        // 
        m_dOptions.inputKind = OPTIX_DENOISER_INPUT_RGB_ALBEDO_NORMAL;
        OPTIX_CHECK(optixDenoiserCreate(m_optixDevice, &m_dOptions, &m_denoiser));
        OPTIX_CHECK(optixDenoiserSetModel(m_denoiser, OPTIX_DENOISER_MODEL_KIND_HDR, nullptr, 0));

        // 
        mIndirect.pixelStrideInBytes = sizeofPixel;
        mIndirect.format = OPTIX_PIXEL_FORMAT_FLOAT4;

        // 
        mNormal.pixelStrideInBytes = sizeofPixel;
        mNormal.format = OPTIX_PIXEL_FORMAT_FLOAT3;

        // 
        mAlbedo.pixelStrideInBytes = sizeofPixel;
        mAlbedo.format = OPTIX_PIXEL_FORMAT_FLOAT4;
    };

    void OptiXDenoise::denoise(vkt::uni_arg<VkCommandBuffer> cmd, vkt::uni_ptr<Framebuffer> framebuffer) {
        mIndirect.rowStrideInBytes = framebuffer->width * mIndirect.pixelStrideInBytes;
        mNormal.rowStrideInBytes = framebuffer->width * mNormal.pixelStrideInBytes;
        mAlbedo.rowStrideInBytes = framebuffer->width * mAlbedo.pixelStrideInBytes;

        // 
        mNormal.width  = mAlbedo.width  = mIndirect.width  = framebuffer->width;
        mNormal.height = mAlbedo.height = mIndirect.height = framebuffer->height;

        // 
        auto interopImage = [&,this](vkt::ImageRegion region, OptixImage2D& image){
            cudaExternalMemoryHandleDesc cudaExtMemHandleDesc{};
#ifdef VKT_WIN32_DETECTED
            cudaExtMemHandleDesc.handle.win32.handle = region->getAllocationInfo().handle;
            cudaExtMemHandleDesc.type = cudaExternalMemoryHandleTypeOpaqueWin32;
#else
            cudaExtMemHandleDesc.handle.fd = -1;
#endif
            // 
            cudaExternalMemory_t cudaExtBuffer{};
            CUDA_CHECK(cudaImportExternalMemory(&cudaExtBuffer, &cudaExtMemHandleDesc));

            // 
            cudaExternalMemoryBufferDesc cudaExtBufferDesc{};
            cudaExtBufferDesc.offset = 0;
            cudaExtBufferDesc.size = region->getAllocationInfo().reqSize;
            cudaExtBufferDesc.flags = 0;
            CUDA_CHECK(cudaExternalMemoryGetMappedBuffer((void**)&image.data, cudaExtBuffer, &cudaExtBufferDesc));
        };

        // 
        interopImage(framebuffer->rasterImages[7], mNormal);
        interopImage(framebuffer->rasterImages[6], mAlbedo);
        interopImage(framebuffer->rasterImages[5], mIndirect);

        // 
    };

#endif

};
