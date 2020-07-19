#pragma once
#include "./Config.hpp"

namespace vlr {

    class Driver : public std::enable_shared_from_this<Driver> { protected: 
        vkt::uni_ptr<xvk::Device> device = {};
        VmaAllocator allocator = {};

    public: 
        Driver(vkt::uni_ptr<xvk::Device> device) { this->constructor(device); };
        Driver() { this->constructor(); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<xvk::Device> device) {
            this->device = device;
        };

        vkt::uni_ptr<xvk::Device> getDevice() const {
            return device;
        };

        VmaAllocator getAllocator() const {
            return allocator;
        };
    };

};
