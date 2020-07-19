#pragma once
#include "./Config.hpp"

namespace vlr {

    class PipelineLayout : public std::enable_shared_from_this<PipelineLayout> { protected: 
        std::vector<VkDescriptorSetLayout> layouts = {};
        VkPipelineLayout bindings = {}, transform = {}; // Unified Bindings, Transform Feedback 
        VkDescriptorSetLayout bindingsDsc = {}, transformDsc = {}; // Dedicated Descriptor Layout

    public: 
        PipelineLayout() { this->constructor(); };
        PipelineLayout(vkt::uni_ptr<Driver> driver) { this->constructor(driver); };

        void constructor() {};
        void constructor(vkt::uni_ptr<Driver> driver) {
            
        };
    };

};
