#pragma once

#include "./vlr/Implementation.hpp"
#include "./vlr/InstanceSet.hpp"
#include "./vlr/Acceleration.hpp"

namespace vlr {

    // 
    void InstanceSet::setAccelerations(std::vector<vkt::uni_ptr<Acceleration>> accelerations) {
        this->accelerations = accelerations;
    };

    // 
    void InstanceSet::pushAcceleration(vkt::uni_ptr<Acceleration> acceleration) {
        this->accelerations.push_back(acceleration);
    };

    // 
    void InstanceSet::resetAccelerations() {
        this->accelerations.clear(); 
        this->accelerations.resize(0u);
    };

};
