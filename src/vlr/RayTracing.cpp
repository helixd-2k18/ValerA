#include "./vlr/RayTracing.hpp"


namespace vlr {

    void RayTracing::constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<PipelineCreateInfo> info) {
        this->driver = driver, this->layout = info->layout, this->framebuffer = info->framebuffer; auto device = this->driver->getDeviceDispatch();

        
    };

};
