#pragma once
#include "./Config.hpp"

namespace vlr {

    class PipelineLayout : public std::enable_shared_from_this<PipelineLayout> { protected: 
        VkPipelineCache cache = {};
        VkPipelineLayout bindings = {}, transform = {}; // Unified Bindings, Transform Feedback 
        VkDescriptorPool pool = {};
        std::vector<VkDescriptorSetLayout> layouts = {};
        std::shared_ptr<Driver> driver = {};
        
    public: 
        PipelineLayout() { this->constructor(); };
        PipelineLayout(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver) {
            // 1. Construct Buffers Set Descriptor Set Layout
            // 2. Construct Texture Set Descriptor Set Layout
            // 3. Construct Sampler Set Descriptor Set Layout
            // 4. Construct Set of Buffers Descriptor Set Layout (when Buffer per Geometry, Node, Acceleration Structure, Instance, etc.)
        };
    };

};
