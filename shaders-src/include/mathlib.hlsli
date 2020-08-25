#ifndef MATHLIB_H
#define MATHLIB_H

#include "./driver.hlsli"
#include "./structs.hlsli"


// 
#if defined(HLSL) || !defined(GLSL)
float uintBitsToFloat(in uint a) { return asfloat(a); };
float2 uintBitsToFloat(in uint2 a) { return asfloat(a); };
float3 uintBitsToFloat(in uint3 a) { return asfloat(a); };
float4 uintBitsToFloat(in uint4 a) { return asfloat(a); };
uint floatBitsToUint(in float a) { return asuint(a); };
uint2 floatBitsToUint(in float2 a) { return asuint(a); };
uint3 floatBitsToUint(in float3 a) { return asuint(a); };
uint4 floatBitsToUint(in float4 a) { return asuint(a); };

// 
uint bitfieldExtract(uint val, int off, int size) {
	// This built-in function is only support in OpenGL 4.0 and ES 3.1
	// Hopefully the shader compiler will get our meaning and emit the right instruction
	uint mask = uint((1 << size) - 1);
	return uint(val >> off) & mask;
};

// 
uint packUint2x16(in uint2 up) {
    return (up.x&0xFFFFu) | ((up.y&0xFFFFu)<<16u);
};

// 
uint2 unpackUint2x16(in uint up) {
    return uint2((up&0xFFFFu), ((up>>16u)&0xFFFFu));
};

// 
uint packUnorm2x16(in float2 fp) {
    return packUint2x16(uint2(fp * 65536.f));
};

// 
float2 unpackUnorm2x16(in uint up) {
    return float2(unpackUint2x16(up)) / 65536.f;
};

// 
uint packUnorm4x8(in float4 color) {
    uint4 colors = uint4(color*255.f)&0xFFu.xxxx;
    return ((colors.x>>0)|(colors.y<<8)|(colors.z<<16)|(colors.w<<24));
};

#define mix lerp
#define fract frac

#else

float asfloat(in uint a) { return uintBitsToFloat(a); };
uint asuint(in float a) { return floatBitsToUint(a); };

uint packUint2x16(in highp uint2 up) {
    return pack32(u16vec2(up));
};

#define lerp mix
#define frac fract

#endif


// 
float4 unpackUnorm4x16(in uint2 c) {
    return float4(unpackUint2x16(c.x), unpackUint2x16(c.y));
};

uint2 packUnorm4x16(in float4 c) {
    return uint2(packUnorm2x16(c.xy), packUnorm2x16(c.zw));
};

// System Specified
#ifdef GLSL
#define meshID nonuniformEXT(geometrySetID)
#else
#define meshID geometrySetID
#endif

// 
#define IndexU8 1000265000
#define IndexU16 0
#define IndexU32 1

// 
#ifndef GLSL
float  fma(in float  a, in float  b, in float  c) { return a*b + c; };
float2 fma(in float2 a, in float2 b, in float2 c) { return a*b + c; };
float3 fma(in float3 a, in float3 b, in float3 c) { return a*b + c; };
float4 fma(in float4 a, in float4 b, in float4 c) { return a*b + c; };
#endif

// 
#ifdef GLSL
float2 lcts(in float3 direct) { return float2(fma(atan(direct.z,direct.x),INV_TWO_PI,0.5f),acos(-direct.y)*INV_PI); };
#else
float2 lcts(in float3 direct) { return float2(fma(atan2(direct.z,direct.x),INV_TWO_PI,0.5f),acos(-direct.y)*INV_PI); };
#endif

// 
float3 dcts(in float2 hr) { 
    hr = fma(hr,float2(TWO_PI,PI),float2(-PI,0.f)); 
    const float up=-cos(hr.y),over=sqrt(fma(up,-up,1.f)); 
    return float3(cos(hr.x)*over,up,sin(hr.x)*over); 
};

float3 exchange(inout float3 orig, in float3 data) {
    float3 old = orig; orig = data; return old;
};

float3 divW(in float4 vect) { return vect.xyz/vect.w; };
float3 divW(in float3 vect) {return vect.xyz; };

uint tiled(in uint x, in uint y) {
    return 1u + ((x - 1u) / y);
};

int tiled(in int x, in int y) {
    return 1 + ((x - 1) / y);
};

#endif

#include "./random.hlsli"
#include "./matrix.hlsli"
#include "./image.hlsli"
#include "./boolean.hlsli"
#include "./intersect.hlsli"
