#ifndef RAYSLIB_H
#define RAYSLIB_H

#include "../include/driver.glsl"
#include "../include/bindings.glsl"

const uint UONE = 1u;
lowp uvec2 bPrefixSum() { return uvec2(subgroupAdd(UONE), subgroupExclusiveAdd(UONE)); };

// 
#define initAtomicSubgroupIncFunction(mem, fname, by, T)\
T fname(in uint WHERE) {\
    const lowp uvec2 pfx = bPrefixSum();\
    T gadd = 0; [[flatten]] if (subgroupElect()) {gadd = atomicAdd(mem, T(pfx.x) * T(by));};\
    return T(pfx.y) * T(by) + readFLane(gadd);\
};

// TODO: Partitioned Atomic Increment...


#endif
