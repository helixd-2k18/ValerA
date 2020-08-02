#pragma once
#include "./Config.hpp"
#include "./Driver.hpp"
#include "./SetBase.hpp"
#include "./Acceleration.hpp"

namespace vlr {
    
    struct BuildCommandCreateInfo {
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<Acceleration> accelerationTop = {};
    };

    class BuildCommand : public std::enable_shared_from_this<BuildCommand> { protected: 
        vkt::uni_ptr<PipelineLayout> layout = {};
        vkt::uni_ptr<Acceleration> accelerationTop = {};
        vkt::uni_ptr<Driver> driver = {};

    public: 
        BuildCommand() { this->constructor(); };
        BuildCommand(vkt::uni_ptr<Driver> driver, vkt::uni_arg<BuildCommandCreateInfo> info = {}) { this->constructor(driver, info); };
        ~BuildCommand() {};

        virtual void constructor() {};
        virtual void constructor(vkt::uni_ptr<Driver> driver, vkt::uni_arg<BuildCommandCreateInfo> info = {});

        // Set Top Level Acceleration
        virtual void setAccelerationTop(vkt::uni_ptr<Acceleration> accelerationTop) {
            this->accelerationTop = accelerationTop;
        };

        //
        virtual void setDescriptorSets(vkt::uni_ptr<PipelineLayout> ilayout = {});
        virtual void setCommand(vkt::uni_arg<VkCommandBuffer> rasterCommand, const glm::uvec4& meta = glm::uvec4(0u));
    };

};
