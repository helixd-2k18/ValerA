#ifndef RAYSLIB_H
#define RAYSLIB_H

#include "./driver.hlsli"
#include "./bindings.hlsli"

const uint UONE = 1u;
uint2 bPrefixSum() { return uvec2(WaveActiveSum(UONE), WavePrefixSum(UONE)); };

// 
#define initAtomicSubgroupIncFunction(mem, fname, by, T)\
T fname(in uint WHERE) {\
    const lowp uvec2 pfx = bPrefixSum();\
    T gadd = 0; [[flatten]] if (WaveIsFirstLane()) {gadd = atomicAdd(mem, T(pfx.x) * T(by));};\
    return T(pfx.y) * T(by) + WaveReadLaneFirst(gadd);\
};

// TODO: Partitioned Atomic Increment...


#endif
