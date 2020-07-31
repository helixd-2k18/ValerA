#ifndef MATHLIB_H
#define MATHLIB_H

#include "./driver.hlsli"

// TODO: Materials
struct RayPayloadData {
     uint4 udata;
    float4 fdata;
    float4 position;
    float4 texcoord;

    float4 normals;
    float4 tangent;
    float4 binorml;
};

struct Binding {
    uint binding;
    uint stride;
    uint rate;
};

struct Attribute {
    uint location;
    uint binding;
    uint format;
    uint offset;
};

struct MaterialUnit {
    float4 diffuse;
    float4 specular;
    float4 normals;
    float4 emission;

    int diffuseTexture;
    int specularTexture;
    int normalsTexture;
    int emissionTexture;

    uint4 udata;
};

struct MeshInfo {
    uint materialID;
    uint indexType;
    uint primitiveCount;
    uint flags;
};

// 
struct RTInstance {
    float3x4 transform;
    uint instance_mask;
    uint offset_flags;
    uint2 handle;
};

// 
struct GeometryDesc {
    float3x4 transform;
    uint firstVertex;
    uint primitiveCount;
    uint material;
    uint meshID_flags; // uint24_t and uint8_t

    // Extended due ray tracing system is limited...
    uint vertexAttribute;
    uint indexBufferView;
    uint indexType; // As Vulkan API TypeLogy 
    uint reserved;
};

struct Interpolations { // Per every geometry for interpolations
    uint AB[8u];
};

struct RayData {
    float3 origin;
    u16vec2 pixelID;
    float3 direct; 
    ubyte4 meta;
    half4 color; half4 emission;
};

struct HitData {
    uint3 indices; uint meshID_meta; // 24-bit MESH ID, 8-bit META
    float3 barycentric; float dist;
};

struct ColorChain {
    float3 color; // RGB Color by RGB32F, Sample Always 1S when has index
    uint next; // Also, can be replaced by Pixel ID when used Accumulation Shader
};


#ifdef GLSL // 4-bit is lifetime
//uint lifetime(in HitData hit) { return bitfieldExtract(hit.meshID_meta, 24, 4); };
//uint lifetime(inout HitData hit, in uint a) { hit.meshID_meta = bitfieldInsert(hit.meshID_meta, a, 24, 4); return a; };
// 
uint lifetime(in RayData ray) { return bitfieldExtract(ray.meta.x, 2, 4); };
uint lifetime(in RayData ray, in uint a) { ray.meta.x = uint8_t(bitfieldInsert(ray.meta.x, a, 2, 4)); return a; };
// 
uint kind(in RayData ray) { return bitfieldExtract(ray.meta.x, 0, 2); };
uint kind(in RayData ray, in uint a) { ray.meta.x = uint8_t(bitfieldInsert(ray.meta.x, a, 0, 2)); return a; };
//
bool finished(in RayData ray) { return bool(bitfieldExtract(ray.meta.x, 6, 1)); };
bool finished(in RayData ray, in bool a) { ray.meta.x = uint8_t(bitfieldInsert(ray.meta.x, uint(a), 6, 1)); return a; };
#endif


//
float3x4 getMT3x4(in float4x3 data) { return transpose(data); };
float3x4 getMT3x4(in float3x4 data) { return data; };
float4x4 getMT4x4(in float4x4 data) { return data; };
float4x4 getMT4x4(in float3x4 data) { return float4x4(data[0], data[1], data[2], float4(0.f,0.f,0.f,1.f)); };
float4x4 getMT4x4(in float4x3 data) { return getMT4x4(transpose(data)); };

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
#else // GLSL-side

float3 mul(in float3 a, in float3x3 m) { return m * a; };
float3 mul(in float3x3 m, in float3 a) { return a * m; };

float4 mul(in float4 a, in float4x4 m) { return m * a; };
float4 mul(in float4x4 m, in float4 a) { return a * m; };

float4 mul(in float3 a, in float3x4 m) { return m * a; };
float4 mul(in float3 a, in float4x3 m) { return a * m; };

float4 mul(in float3x4 m, in float3 a) { return m * a; };
float3 mul(in float3x4 m, in float4 a) { return a * m; };

float3 mul(in float4x3 m, in float4 a) { return m * a; };
float4 mul(in float4x3 m, in float3 a) { return a * m; };


float asfloat(in uint a) { return uintBitsToFloat(a); };
uint asuint(in float a) { return floatBitsToUint(a); };

uint packUint2x16(in highp uint2 up) {
    return pack32(u16vec2(up));
};
#endif


float4 unpackUnorm4x16(in uint2 c) {
    return float4(unpackUint2x16(c.x), unpackUint2x16(c.y));
};

uint2 packUnorm4x16(in float4 c) {
    return uint2(packUnorm2x16(c.xy), packUnorm2x16(c.zw));
};


// 
bool hasTransform(in GeometryDesc desc) {
    return bool(bitfieldExtract(desc.meshID_flags,24+0,1));
};

bool hasNormal(in GeometryDesc desc) {
    return bool(bitfieldExtract(desc.meshID_flags,24+1,1));
};

bool hasTexcoord(in GeometryDesc desc) {
    return bool(bitfieldExtract(desc.meshID_flags,24+2,1));
};

bool hasTangent(in GeometryDesc desc) {
    return bool(bitfieldExtract(desc.meshID_flags,24+3,1));
};

// color space utils
STATIC const float HDR_GAMMA = 2.2f;

// 
#ifndef GLSL
bool3 lessThan(in float3 a, in float3 b) { return a < b; };
#endif

// 
float3 fromLinear(in float3 linearRGB) { return mix(float3(1.055)*pow(linearRGB, float3(1.0/2.4)) - float3(0.055), linearRGB * float3(12.92), lessThan(linearRGB, float3(0.0031308))); }
float3 toLinear(in float3 sRGB) { return mix(pow((sRGB + float3(0.055))/float3(1.055), float3(2.4)), sRGB/float3(12.92), lessThan(sRGB, float3(0.04045))); }
float4 fromLinear(in float4 linearRGB) { return float4(fromLinear(linearRGB.xyz), linearRGB.w); }
float4 toLinear(in float4 sRGB) { return float4(toLinear(sRGB.xyz), sRGB.w); }





//
#ifndef GLSL
#define imageLoad(b, c) b[c]
//#define imageStore(b, c, f) (b[c] = f)
//#define texelFetch(b, c) b[c]
#define texelFetch(b, c, m) b[c]//b.Load(c,uint(m))
#define textureSample(b, s, c) b.Sample(s, c)
//#define textureSample(b, s, c, m) b.SampleLevel(s, c, m)
#define textureLodSample(b, s, c, m) b.SampleLevel(s, c, m)
#define nonuniformEXT(a) a

// 
#define fimage2D RWTexture2D<float>
void imageStore(in RWTexture2D<float> b, in int2 c, in float4 f) { b[c] = f; };
void imageStore(in RWTexture2D<float4> b, in int2 c, in float4 f) { b[c] = f; };
void imageStore(in RWTexture3D<float4> b, in int3 c, in float4 f) { b[c] = f; };
uint2 imageSize(in RWTexture2D<float4> tex) { uint2 size = uint2(0,0); tex.GetDimensions(size.x, size.y); return size; };
uint3 imageSize(in RWTexture3D<float4> tex) { uint3 size = uint3(0,0,0); tex.GetDimensions(size.x, size.y, size.z); return size; };
uint2 textureSize(in Texture2D<float4> tex, in int lod) { uint2 size = uint2(0,0); tex.GetDimensions(size.x, size.y); return size; };
uint3 textureSize(in Texture3D<float4> tex, in int lod) { uint3 size = uint3(0,0,0); tex.GetDimensions(size.x, size.y, size.z); return size; };
//uint2 textureSize(in Texture2D tex, in int lod) { uint2 size = uint2(0,0); return tex.GetDimensions(uint(lod), size.x, size.y); return size; };
//uint3 textureSize(in Texture3D tex, in int lod) { uint3 size = uint3(0,0,0); return tex.GetDimensions(uint(lod), size.x, size.y, size.z); return size; };

#else
#define fimage2D image2D
//
#define textureSample(b, s, c) texture(sampler2D(b, s), c)
#define textureLodSample(b, s, c, m) textureLod(sampler2D(b, s), c, m)
#endif

#define superImageLoad(image, texcoord) \
    float4(\
        imageLoad(image, int2(texcoord.x*4u+0u,texcoord.y)).x,\
        imageLoad(image, int2(texcoord.x*4u+1u,texcoord.y)).x,\
        imageLoad(image, int2(texcoord.x*4u+2u,texcoord.y)).x,\
        imageLoad(image, int2(texcoord.x*4u+3u,texcoord.y)).x\
    )

#define superImageStore(image, texcoord, fvalue) \
    imageStore(image, int2(texcoord.x*4u+0u,texcoord.y), fvalue.xxxx);\
    imageStore(image, int2(texcoord.x*4u+1u,texcoord.y), fvalue.yyyy);\
    imageStore(image, int2(texcoord.x*4u+2u,texcoord.y), fvalue.zzzz);\
    imageStore(image, int2(texcoord.x*4u+3u,texcoord.y), fvalue.wwww);

#ifdef GLSL 
#define atomicSuperImageAdd(image, texcoord, fvalue) \
    float4(\
        imageAtomicAdd(image, int2(texcoord.x*4u+0u,texcoord.y), fvalue.x),\
        imageAtomicAdd(image, int2(texcoord.x*4u+1u,texcoord.y), fvalue.y),\
        imageAtomicAdd(image, int2(texcoord.x*4u+2u,texcoord.y), fvalue.z),\
        imageAtomicAdd(image, int2(texcoord.x*4u+3u,texcoord.y), fvalue.w) \
    )

#define atomicSuperImageAdd3(image, texcoord, fvalue) \
    float4(\
        imageAtomicAdd(image, int2(texcoord.x*4u+0u,texcoord.y), fvalue.x),\
        imageAtomicAdd(image, int2(texcoord.x*4u+1u,texcoord.y), fvalue.y),\
        imageAtomicAdd(image, int2(texcoord.x*4u+2u,texcoord.y), fvalue.z),\
        imageAtomicExchange(image, int2(texcoord.x*4u+3u,texcoord.y), 1.f)\
    )

#endif


// System Specified
#ifdef GLSL
#define meshID nonuniformEXT(geometrySetID)
#else
#define meshID geometrySetID
#endif



float4x4 regen4(in float3x4 T) {
    return float4x4(T[0],T[1],T[2],float4(0.f.xxx,1.f));
};

float3x3 regen3(in float3x4 T) {
    return float3x3(T[0].xyz,T[1].xyz,T[2].xyz);
};

float4 mul4(in float4 v, in float3x4 M) {
    return float4(mul(M, v),1.f);
};

// 
#define IndexU8 1000265000
#define IndexU16 0
#define IndexU32 1

// 
float raySphereIntersect(in float3 r0, in float3 rd, in float3 s0, in float sr) {
    float a = dot(rd, rd);
    float3 s0_r0 = r0 - s0;
    float b = 2.0 * dot(rd, s0_r0);
    float c = dot(s0_r0, s0_r0) - (sr * sr);
    if (b*b - 4.0*a*c < 0.0) {
        return -1.0;
    }
    return (-b - sqrt((b*b) - 4.0*a*c))/(2.0*a);
};

// NEXT standard consts in current
// Ray tracing NEXT capable shader standard development planned begin in 2019 year
STATIC const float PHI = 1.6180339887498948482f;
STATIC const float SFN = 0.00000011920928955078125f, SFO = 1.f+SFN;//1.00000011920928955078125f;
STATIC const float INFINITY = 1e+5f, N_INFINITY = (INFINITY*(1.f-SFN));
STATIC const float PI = 3.1415926535897932384626422832795028841971f;
STATIC const float TWO_PI = 6.2831853071795864769252867665590057683943f;
STATIC const float SQRT_OF_ONE_THIRD = 0.5773502691896257645091487805019574556476f;
STATIC const float E = 2.7182818284590452353602874713526624977572f;
STATIC const float INV_PI = 0.3183098861837907f;
STATIC const float TWO_INV_PI = 0.6366197723675814f;
STATIC const float INV_TWO_PI = 0.15915494309189535f;

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

float2 flip(in float2 texcoord) { return float2(texcoord.x, 1.f - texcoord.y); };

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

bool4 and(in bool4 a, in bool4 b){
    return bool4(a.x&&b.x,a.y&&b.y,a.z&&b.z,a.w&&b.w);
};

bool4 or(in bool4 a, in bool4 b){
    return bool4(a.x||b.x,a.y||b.y,a.z||b.z,a.w||b.w);
};

bool3 and(in bool3 a, in bool3 b){
    return bool3(a.x&&b.x,a.y&&b.y,a.z&&b.z);
};

bool3 or(in bool3 a, in bool3 b){
    return bool3(a.x||b.x,a.y||b.y,a.z||b.z);
};

bool fequal(in float a, in float b){
    return 
        a <= b + 0.0001f && 
        a >= b - 0.0001f;
};


#if defined(HLSL) || !defined(GLSL)

bool2 fequal(in float2 a, in float2 b){
    return a <= (b + 0.0001f.xx) && a >= (b - 0.0001f.xx);
};

bool3 fequal(in float3 a, in float3 b){
    return a <= (b + 0.0001f.xxx) && a >= (b - 0.0001f.xxx);
};

bool4 fequal(in float4 a, in float4 b){
    return a <= (b + 0.0001f.xxxx) && a >= (b - 0.0001f.xxxx);
};

#else

bool3 fequal(in float3 a, in float3 b){
    return and(
        lessThanEqual(a, b + 0.0001f),
        greaterThanEqual(a, b - 0.0001f));
};

bool4 fequal(in float4 a, in float4 b){
    return and(
        lessThanEqual(a, b + 0.0001f),
        greaterThanEqual(a, b - 0.0001f));
};

#endif


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

float planeIntersect(in float3 l0, in float3 r, in float3 p0, in float3 n){
    return dot(p0-l0,n)/dot(r,n);
};

float3 exchange(inout float3 orig, in float3 data) {
    float3 old = orig; orig = data; return old;
};

float3 divW(in float4 vect) { return vect.xyz/vect.w; };
float3 divW(in float3 vect) {return vect.xyz; };



#ifndef GLSL

#define IDENTITY_MATRIX float4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)

float2x2 inverse(in float2x2 m) 
{
    return float2x2(m[1][1],-m[0][1],
                    -m[1][0], m[0][0]) / (m[0][0]*m[1][1] - m[0][1]*m[1][0]);
};

float3x3 inverse(in float3x3 m) 
{
    float a00 = m[0][0], a01 = m[0][1], a02 = m[0][2];
    float a10 = m[1][0], a11 = m[1][1], a12 = m[1][2];
    float a20 = m[2][0], a21 = m[2][1], a22 = m[2][2];

    float b01 = a22 * a11 - a12 * a21;
    float b11 = -a22 * a10 + a12 * a20;
    float b21 = a21 * a10 - a11 * a20;

    float det = a00 * b01 + a01 * b11 + a02 * b21;

    return float3x3(b01, (-a22 * a01 + a02 * a21), (a12 * a01 - a02 * a11),
                    b11, (a22 * a00 - a02 * a20), (-a12 * a00 + a02 * a10),
                    b21, (-a21 * a00 + a01 * a20), (a11 * a00 - a01 * a10)) / det;
};

float4x4 inverse(in float4x4 m) 
{
    float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
    float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
    float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
    float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

    float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    float idet = 1.0f / det;

    float4x4 ret;

    ret[0][0] = t11 * idet;
    ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
    ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
    ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

    ret[1][0] = t12 * idet;
    ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
    ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
    ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

    ret[2][0] = t13 * idet;
    ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
    ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
    ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

    ret[3][0] = t14 * idet;
    ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
    ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
    ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

    return ret;
};

// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
float4 matrix_to_quaternion(in float4x4 m)
{
    float tr = m[0][0] + m[1][1] + m[2][2];
    float4 q = float4(0, 0, 0, 0);

    if (tr > 0)
    {
        float s = sqrt(tr + 1.0) * 2; // S=4*qw 
        q.w = 0.25 * s;
        q.x = (m[2][1] - m[1][2]) / s;
        q.y = (m[0][2] - m[2][0]) / s;
        q.z = (m[1][0] - m[0][1]) / s;
    }
    else if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2]))
    {
        float s = sqrt(1.0 + m[0][0] - m[1][1] - m[2][2]) * 2; // S=4*qx 
        q.w = (m[2][1] - m[1][2]) / s;
        q.x = 0.25 * s;
        q.y = (m[0][1] + m[1][0]) / s;
        q.z = (m[0][2] + m[2][0]) / s;
    }
    else if (m[1][1] > m[2][2])
    {
        float s = sqrt(1.0 + m[1][1] - m[0][0] - m[2][2]) * 2; // S=4*qy
        q.w = (m[0][2] - m[2][0]) / s;
        q.x = (m[0][1] + m[1][0]) / s;
        q.y = 0.25 * s;
        q.z = (m[1][2] + m[2][1]) / s;
    }
    else
    {
        float s = sqrt(1.0 + m[2][2] - m[0][0] - m[1][1]) * 2; // S=4*qz
        q.w = (m[1][0] - m[0][1]) / s;
        q.x = (m[0][2] + m[2][0]) / s;
        q.y = (m[1][2] + m[2][1]) / s;
        q.z = 0.25 * s;
    }

    return q;
};

float4x4 m_scale(in float4x4 m, in float3 v)
{
    float x = v.x, y = v.y, z = v.z;

    m[0][0] *= x; m[1][0] *= y; m[2][0] *= z;
    m[0][1] *= x; m[1][1] *= y; m[2][1] *= z;
    m[0][2] *= x; m[1][2] *= y; m[2][2] *= z;
    m[0][3] *= x; m[1][3] *= y; m[2][3] *= z;

    return m;
};

float4x4 quaternion_to_matrix(in float4 quat)
{
    float4x4 m = float4x4(float4(0, 0, 0, 0), float4(0, 0, 0, 0), float4(0, 0, 0, 0), float4(0, 0, 0, 0));

    float x = quat.x, y = quat.y, z = quat.z, w = quat.w;
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, xy = x * y2, xz = x * z2;
    float yy = y * y2, yz = y * z2, zz = z * z2;
    float wx = w * x2, wy = w * y2, wz = w * z2;

    m[0][0] = 1.0 - (yy + zz);
    m[0][1] = xy - wz;
    m[0][2] = xz + wy;

    m[1][0] = xy + wz;
    m[1][1] = 1.0 - (xx + zz);
    m[1][2] = yz - wx;

    m[2][0] = xz - wy;
    m[2][1] = yz + wx;
    m[2][2] = 1.0 - (xx + yy);

    m[3][3] = 1.0;

    return m;
};

float4x4 m_translate(in float4x4 m, in float3 v)
{
    float x = v.x, y = v.y, z = v.z;
    m[0][3] = x;
    m[1][3] = y;
    m[2][3] = z;
    return m;
};

float4x4 compose(in float3 position, in float4 quat, in float3 scale)
{
    float4x4 m = quaternion_to_matrix(quat);
    m = m_scale(m, scale);
    m = m_translate(m, position);
    return m;
};

void decompose(in float4x4 m, out float3 position, out float4 rotation, out float3 scale)
{
    float sx = length(float3(m[0][0], m[0][1], m[0][2]));
    float sy = length(float3(m[1][0], m[1][1], m[1][2]));
    float sz = length(float3(m[2][0], m[2][1], m[2][2]));

    // if determine is negative, we need to invert one scale
    float det = determinant(m);
    if (det < 0) {
        sx = -sx;
    }

    position.x = m[3][0];
    position.y = m[3][1];
    position.z = m[3][2];

    // scale the rotation part

    float invSX = 1.0 / sx;
    float invSY = 1.0 / sy;
    float invSZ = 1.0 / sz;

    m[0][0] *= invSX;
    m[0][1] *= invSX;
    m[0][2] *= invSX;

    m[1][0] *= invSY;
    m[1][1] *= invSY;
    m[1][2] *= invSY;

    m[2][0] *= invSZ;
    m[2][1] *= invSZ;
    m[2][2] *= invSZ;

    rotation = matrix_to_quaternion(m);

    scale.x = sx;
    scale.y = sy;
    scale.z = sz;
};

float4x4 axis_matrix(in float3 right, in float3 up, in float3 forward)
{
    float3 xaxis = right;
    float3 yaxis = up;
    float3 zaxis = forward;
    return float4x4(
		xaxis.x, yaxis.x, zaxis.x, 0,
		xaxis.y, yaxis.y, zaxis.y, 0,
		xaxis.z, yaxis.z, zaxis.z, 0,
		0, 0, 0, 1
	);
};

// http://stackoverflow.com/questions/349050/calculating-a-lookat-matrix
float4x4 look_at_matrix(in float3 forward, in float3 up)
{
    float3 xaxis = normalize(cross(forward, up));
    float3 yaxis = up;
    float3 zaxis = forward;
    return axis_matrix(xaxis, yaxis, zaxis);
};

float4x4 look_at_matrix(in float3 at, in float3 eye, in float3 up)
{
    float3 zaxis = normalize(at - eye);
    float3 xaxis = normalize(cross(up, zaxis));
    float3 yaxis = cross(zaxis, xaxis);
    return axis_matrix(xaxis, yaxis, zaxis);
};

float4x4 extract_rotation_matrix(in float4x4 m)
{
    float sx = length(float3(m[0][0], m[0][1], m[0][2]));
    float sy = length(float3(m[1][0], m[1][1], m[1][2]));
    float sz = length(float3(m[2][0], m[2][1], m[2][2]));

    // if determine is negative, we need to invert one scale
    float det = determinant(m);
    if (det < 0) { sx = -sx; };

    float invSX = 1.0 / sx;
    float invSY = 1.0 / sy;
    float invSZ = 1.0 / sz;

    m[0][0] *= invSX;
    m[0][1] *= invSX;
    m[0][2] *= invSX;
    m[0][3] = 0;

    m[1][0] *= invSY;
    m[1][1] *= invSY;
    m[1][2] *= invSY;
    m[1][3] = 0;

    m[2][0] *= invSZ;
    m[2][1] *= invSZ;
    m[2][2] *= invSZ;
    m[2][3] = 0;

    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;

    return m;
};

#endif

// 
float4x4 inverse(in float3x4 imat) {
    //return inverse(transpose(float4x4(imat[0],imat[1],imat[2],float4(0.f,0.f,0.f,1.f))));
    return transpose(inverse(float4x4(imat[0],imat[1],imat[2],float4(0.f,0.f,0.f,1.f))));
};

uint tiled(in uint x, in uint y) {
    return 1u + ((x - 1u) / y);
};

int tiled(in int x, in int y) {
    return 1 + ((x - 1) / y);
};

#endif
