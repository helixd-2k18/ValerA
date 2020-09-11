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

// 
initAtomicSubgroupIncFunction(counters[WHERE], incrementCnt, 1u, uint);

// 
const uint LIMITS = 4096u * 4096u;
const uint RAY_COUNTER = 0u;
const uint RAY_COUNTER_READ = 1u;
const uint HIT_COUNTER = 2u;
const uint HIT_COUNTER_READ = 3u;


const uint DIFFUSE_RAY = 0u;
const uint REFLECT_RAY = 1u;
const uint SHADOWS_RAY = 2u;
const uint TRANSPC_RAY = 3u;

// 
RayData finishRay(inout RayData ray) {
#ifdef GLSL
    finished(ray, true);
    if (dot(ray.emission.xyz, 1.hf.xxx) > 0.001hf || ray.emission.w > 0.001hf) {
        uint kindof = kind(ray); ray.emission /= max(ray.emission.w, 1.hf);
        if (kindof == REFLECT_RAY) { atomicSuperImageAdd(currImages[nonuniformEXT(IW_REFLECLR)], int2(ray.pixelID), float4(min(ray.emission, half4(8.f.xxx, 1.f)))); };
        if (kindof == DIFFUSE_RAY) { atomicSuperImageAdd(currImages[nonuniformEXT(IW_INDIRECT)], int2(ray.pixelID), float4(min(ray.emission, half4(8.f.xxx, 1.f)))); };
        if (kindof == TRANSPC_RAY) { atomicSuperImageAdd(currImages[nonuniformEXT(IW_TRANSPAR)], int2(ray.pixelID), float4(min(ray.emission, half4(8.f.xxx, 1.f)))); };
        if (kindof == SHADOWS_RAY) { atomicSuperImageAdd(currImages[nonuniformEXT(IW_SHADOWCL)], int2(ray.pixelID), float4(min(ray.emission, half4(8.f.xxx, 1.f)))); };
        ray.emission.xyzw = 0.hf.xxxx;
    };
#endif
    return ray;
};

// 
RayData finishBy(inout RayData ray) {
#ifdef GLSL
    if (dot(ray.color.xyz, 1.hf.xxx) <= 0.001f || lifetime(ray) <= 0) { finishRay(ray); };
#endif
    return ray;
};

// 
void storeRay(in RayData ray, in uint rayID) {
#ifdef GLSL
    if (rayID != ~0u && rayID < LIMITS) { lifetime(ray, lifetime(ray)-1); rays[0u].data[rayID] = ray; };
#endif
};

// 
uint emitRay(inout RayData ray) {
    uint rayID = ~0u; finishBy(ray);
#ifdef GLSL
    if (counters[RAY_COUNTER] < LIMITS && !finished(ray) && lifetime(ray) > 0u) 
    {
        rayID = incrementCnt(RAY_COUNTER);
        storeRay(ray, rayID);
    };
#endif
    return rayID;
};

// 
void storeHit(in HitData hit, in uint hitID) { 
    if (hitID != ~0u && hitID < LIMITS) { hits[0u].data[hitID] = hit; };
};

// 
uint emitHit(in HitData hit) {
    uint hitID = ~0u;
#ifdef GLSL
    if (counters[HIT_COUNTER] < LIMITS) { 
        hitID = incrementCnt(HIT_COUNTER);
        storeHit(hit, hitID);
    };
#endif
    return hitID;
};



#endif
