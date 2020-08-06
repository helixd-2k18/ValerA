#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"

#ifdef ENABLE_OPTIX_DENOISE
#include <cuda.h>
#include <cuda_runtime.h>
#include <optix/optix.h>
#include <optix/optix_stubs.h>
#include <optix/optix_types.h>
#endif

namespace vlr {

#ifdef ENABLE_OPTIX_DENOISE
    class OptiXDenoise : public std::enable_shared_from_this<OptiXDenoise> { protected: 
        OptixDenoiser        m_denoiser{ nullptr };
        OptixDenoiserOptions m_dOptions{};
        OptixDenoiserSizes   m_dSizes{};
        CUdeviceptr          m_dState{ 0 };
        CUdeviceptr          m_dScratch{ 0 };
        CUdeviceptr          m_dIntensity{ 0 };
        CUdeviceptr          m_dMinRGB{ 0 };

        OptixImage2D         mIndirect{};
        OptixImage2D         mNormal{};
        OptixImage2D         mAlbedo{};
        OptixImage2D         mOutput{};

        VkDeviceSize         recommendedScratchSizeInBytes = 0u;

    public: 
        OptiXDenoise() { this->constructor(); };
        OptiXDenoise(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver);

        // 
        virtual void setFramebuffer(vkt::uni_ptr<Framebuffer> framebuffer);
        virtual void denoise();
    };
#endif

};
