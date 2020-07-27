#pragma once
#include "./Config.hpp"
#include "./SetBase.hpp"
#include "./Acceleration.hpp"

namespace vlr {
    
    struct BuildCommandCreateInfo {
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<Acceleration> accelerationTop = {};
        std::vector<vkt::uni_ptr<Acceleration>> accelerations = {};
    };

    class BuildCommand : public std::enable_shared_from_this<BuildCommand> { protected: 
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<Acceleration> accelerationTop = {};
        std::vector<vkt::uni_ptr<Acceleration>> accelerations = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        BuildCommand() { this->constructor(); };
        BuildCommand(vkt::uni_ptr<Driver> driver, vkt::uni_arg<BuildCommandCreateInfo> info = {}) { this->constructor(driver, info); };

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<BuildCommandCreateInfo> info = {});
        
        // Set Top Level Acceleration
        virtual void setAccelerationTop(vkt::uni_ptr<Acceleration> accelerationTop) {
            this->accelerationTop = accelerationTop;
        };

        // Push Bottom Level Acceleration (with Geometry Set)
        virtual void pushAcceleration(vkt::uni_ptr<Acceleration> acceleration) {
            this->accelerations.push_back(acceleration);
        };

        //
        virtual void setDescriptorSets(vkt::uni_ptr<PipelineLayout> ilayout = {});
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> rasterCommand, const glm::uvec4& meta = glm::uvec4(0u));
    };

};
