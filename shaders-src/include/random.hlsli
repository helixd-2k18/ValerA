#ifndef RANDOM_H
#define RANDOM_H

#include "./mathlib.hlsli"

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint hash( uint x ) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}

// Compound versions of the hashing algorithm I whipped together.
STATIC uint counter = 0u;
uint hash( uint2 v ) { return hash( hash(counter++) ^ v.x ^ hash(v.y)                         ); }
uint hash( uint3 v ) { return hash( hash(counter++) ^ v.x ^ hash(v.y) ^ hash(v.z)             ); }
uint hash( uint4 v ) { return hash( hash(counter++) ^ v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w) ); }

// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct( uint m ) {
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

#ifdef GLSL
    float  f = uintBitsToFloat( m );       // Range [1:2]
    return fract(f - 1.0);                 // Range [0:1]
#else
    float  f = asfloat( m );       // Range [1:2]
    return frac(f - 1.0);          // Range [0:1]
#endif
};

// 
#ifdef GLSL
highp float2 halfConstruct ( in uint  m ) { return fract(unpackHalf2x16((m & 0x03FF03FFu) | (0x3C003C00u))-1.f); }
#else
float2 halfConstruct ( in uint  m ) { return frac(f16tof32((m & 0x03FF03FFu) | (0x3C003C00u))-1.f); }
#endif


// Pseudo-random value in half-open range [0:1].
//float random( float x ) { return floatConstruct(hash(floatBitsToUint(x))); }
//float random( float2  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
//float random( float3  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
//float random( float4  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
#ifdef GLSL
#define QLOCK2 (clockRealtime2x32EXT()+clock2x32ARB())
#else
#define QLOCK2 0u.xx
#endif

STATIC uint SCLOCK = 0u;
//#define QLOCK2 uint2(0u,0u)

// 
float random(               ) {         return floatConstruct(hash(QLOCK2)); }
float random( inout uint2 s ) { s += 1; return floatConstruct(hash(uint4(QLOCK2,s))); }
float random( inout uint  s ) { s += 1; return floatConstruct(hash(uint3(QLOCK2,s))); }

// 
float2 random2(               ) {         return halfConstruct(hash(QLOCK2)); }
float2 random2( inout uint2 s ) { s += 1; return halfConstruct(hash(uint4(QLOCK2,s))); }
float2 random2( inout uint  s ) { s += 1; return halfConstruct(hash(uint3(QLOCK2,s))); }

// pseudo random
float2 pRandom2( inout uint2 s ) { s += 1; return halfConstruct(hash(s)); }
float2 pRandom2( inout uint  s ) { s += 1; return halfConstruct(hash(s)); }


// geometric random generators
//float3 randomSphere() { return dcts(random2()); };
//float3 randomSphere(in uint  s) { return dcts(random2(s)); };
//float3 randomSphere(in uint2 s) { return dcts(random2(s)); };


float3 randomSphere( inout uint2 seed ) {
    float up = random(seed) * 2.0 - 1.0; // range: -1 to +1
    float over = sqrt( max(0.0, 1.0 - up * up) );
    float around = random(seed) * TWO_PI;
    return normalize(float3(cos(around) * over, up, sin(around) * over));	
};

float3 randomHemisphereCosine(inout uint2 seed) {
    const float2 hmsm = random2(seed);
    const float phi = hmsm.x * TWO_PI, up = sqrt(1.0f - hmsm.y), over = sqrt(fma(up,-up,1.f));
    return normalize(float3(cos(phi)*over,up,sin(phi)*over));
};


/*
float3 randomHemisphereCosine(inout uint2 seed, in float3x3 TBN) {
    return normalize(TBN * randomHemisphereCosine(seed).xzy);
};
*/

/*
float3 randomHemisphereCosine(inout uint2 seed, in float3 tangent, in float3 n) {
    float3 up = abs(n.z) < 0.999f ? float3(0.f, 0.f, 1.f) : float3(1.f, 0.f, 0.f);
	float3 tan_x = normalize(cross(up, n));
	float3 tan_y = cross(n, tan_x);
    float3 hemi = randomHemisphereCosine(seed);
    return normalize(hemi.x * tan_x + hemi.y * tan_y + n * hemi.z);
};
*/

float3 randomHemisphereCosine( inout uint2 seed, in float3x3 TBN)
{
    float up = random(seed); // uniform distribution in hemisphere
    float over = sqrt(max(0.0, 1.0 - up * up));
    float around = random(seed) * TWO_PI;
    // from "Building an Orthonormal Basis, Revisited" http://jcgt.org/published/0006/01/01/
    float signf = TBN[2].z >= 0.0 ? 1.0 : -1.0;
    float a = -1.0 / (signf + TBN[2].z);
    float b = TBN[2].x * TBN[2].y * a;
    float3 T = float3( 1.0 + signf * TBN[2].x * TBN[2].x * a, signf * b, -signf * TBN[2].x );
    float3 B = float3( b, signf + TBN[2].y * TBN[2].y * a, -TBN[2].y );
    return normalize(cos(around) * over * T + sin(around) * over * B + up * TBN[2]);
};


float3 reflectGlossy(inout uint2 seed, in float3 I, in float3x3 TBN, in float gloss){
    return normalize(mix(reflect(I,TBN[2]), randomHemisphereCosine(seed,TBN), gloss*sqrt(random(seed))));
};


#endif
