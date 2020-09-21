#include "./vlr/Implementation.hpp"
#include "./vlr/VertexSet.hpp"

namespace vlr {

    void VertexSet::createDescriptorSet(vkt::uni_ptr<PipelineLayout> pipelineLayout) {
        this->bindings->createDescriptorSet(pipelineLayout);
        this->attributes->createDescriptorSet(pipelineLayout);
        this->bufferViews->createDescriptorSet(pipelineLayout);
    };

};
