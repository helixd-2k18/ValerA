#ifndef RAYSLIB_H
#define RAYSLIB_H

#include "./driver.hlsli"
#include "./mathlib.hlsli"
#include "./bindings.hlsli"

// 
#ifdef GLSL
#define WaveActiveSum subgroupAdd
#define WavePrefixSum subgroupExclusiveAdd
#define WaveIsFirstLane subgroupElect
#define WaveReadLaneFirst subgroupBroadcastFirst
#endif

// 
const uint UONE = 1u;
uint2 bPrefixSum() { return uvec2(WaveActiveSum(UONE), WavePrefixSum(UONE)); };

// 
#define initAtomicSubgroupIncFunction(mem, fname, by, T)\
T fname(in uint WHERE) {\
    const lowp uvec2 pfx = bPrefixSum();\
    T gadd = 0; [[flatten]] if (WaveIsFirstLane()) {gadd = atomicAdd(mem, T(pfx.x) * T(by));};\
    return T(pfx.y) * T(by) + WaveReadLaneFirst(gadd);\
};

initAtomicSubgroupIncFunction(counters[WHERE], incrementCnt, 1u, uint);

// TODO: Partitioned Atomic Increment...

const uint LIMITS = 4096u * 4096u;
const uint RAY_COUNTER = 0u;
const uint RAY_COUNTER_READ = 1u;
const uint HIT_COUNTER = 2u;
const uint HIT_COUNTER_READ = 3u;

// 
RayData finishRay(inout RayData ray) {
#ifdef GLSL
    finished(ray, true);
    if (dot(ray.emission.xyz, 0.hf.xxx) > 0.001hf) {
        atomicSuperImageAdd3(currBuffers[IW_INDIRECT], int2(ray.pixelID), ray.emission.xyz);
        ray.emission.xyz = 0.hf.xxx
    };
#endif
    return ray;
};

// 
RayData finishBy(inout RayData ray) {
    if (dot(ray.color.xyz, 0.hf.xxx) <= 0.001f || lifetime(ray) <= 0u) { finishRay(ray); }; return ray;
};

// 
uint emitRayT(in RayData ray) {
    uint rayID = ~0u;
#ifdef GLSL
    if (counters[RAY_COUNTER] < LIMITS && !finished(ray) && lifetime(ray) > 0u) {
        rayID = incrementCnt(RAY_COUNTER);
    };
    if (rayID != ~0u && rayID < LIMITS) {
        lifetime(ray, lifetime(ray)-1u);
        rays[rayID] = ray;
    };
#endif
    return rayID;
};

uint emitRay(inout RayData ray){
    return emitRayT(finishBy(ray));
};


// 
uint emitHit(in HitData hit) {
    uint hitID = ~0u;
#ifdef GLSL
    if (counters[RAY_COUNTER] < LIMITS) {
        hitID = incrementCnt(HIT_COUNTER);
    };
    if (hitID != ~0u && hitID < LIMITS) {
        hits[hitID] = hit;
    };
#endif
    return hitID;
};



#endif
