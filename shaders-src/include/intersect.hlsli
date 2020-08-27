#ifndef INTERSECT_H
#define INTERSECT_H

#include "./mathlib.hlsli"


struct Box { float3 min, max; };

float2 boxIntersect(in float3 rayOrigin, in float3 rayDir, in float3 boxMin, in float3 boxMax) {
    float3 tMin = (boxMin - rayOrigin) / rayDir;
    float3 tMax = (boxMax - rayOrigin) / rayDir;
    float3 t1 = min(tMin, tMax);
    float3 t2 = max(tMin, tMax);
    float tNear = max(max(t1.x, t1.y), t1.z);
    float tFar = min(min(t2.x, t2.y), t2.z);
    return float2(tNear, tFar);
    //return 0.f.xx;
};

float3 boxNormal(in float3 mpoint, in float3 boxMin, in float3 boxMax) {
    const float kEpsilon = 0.0001f;
	float3 center = (boxMax + boxMin) * 0.5f;
	float3 size = (boxMax - boxMin) * 0.5f;
	float3 pc = mpoint - center;
	float3 normal = float3(0.0f.xxx);
	normal += float3(sign(pc.x), 0.0f, 0.0f) * step(abs(abs(pc.x) - size.x), kEpsilon);
	normal += float3(0.0f, sign(pc.y), 0.0f) * step(abs(abs(pc.y) - size.y), kEpsilon);
	normal += float3(0.0f, 0.0f, sign(pc.z)) * step(abs(abs(pc.z) - size.z), kEpsilon);
	return normalize(normal);
};

// 
float raySphereIntersect(in float3 r0, in float3 rd, in float3 s0, in float sr) {
    float a = dot(rd, rd);
    float3 s0_r0 = r0 - s0;
    float b = 2.0f * dot(rd, s0_r0);
    float c = dot(s0_r0, s0_r0) - (sr * sr);
    if (b*b - 4.0f*a*c < 0.0f) { return 10000.0f; };
    float d1 = (-b - sqrt((b*b) - 4.0f*a*c))/(2.0f*a);
    float d2 = (-b + sqrt((b*b) - 4.0f*a*c))/(2.0f*a);
    float mn = min(d1,d2), mx = max(d1,d2);
    return mx >= 0.f ? (mn >= 0.f ? mn : mx) : 10000.f;
};

float planeIntersect(in float3 l0, in float3 r, in float3 p0, in float3 n){
    return dot(p0-l0,n)/dot(r,n);
};

#endif
