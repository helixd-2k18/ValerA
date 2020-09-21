// #
#if defined(_WIN32) && !defined(WIN32)
#define WIN32
#endif

//
#ifdef WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif

//
//#define VMA_IMPLEMENTATION
#define TINYEXR_IMPLEMENTATION

#include "./vlr/Config.hpp"
#include "./vlr/Implementation.hpp"

namespace vlr {

};
