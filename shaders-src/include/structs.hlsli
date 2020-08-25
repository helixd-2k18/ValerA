#ifndef STRUCTS_H
#define STRUCTS_H

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

    uint attributes[8u];
};

struct RayData {
    float3 origin;
    u16vec2 pixelID;
    float3 direct; 
    //ubyte4 meta;
    u8vec4 meta;
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


// 
#ifdef GLSL // 4-bit is lifetime
//uint lifetime(in HitData hit) { return bitfieldExtract(hit.meshID_meta, 24, 4); };
//uint lifetime(inout HitData hit, in uint a) { hit.meshID_meta = bitfieldInsert(hit.meshID_meta, a, 24, 4); return a; };
// 
const int fb=0;
uint lifetime(inout RayData ray) { return bitfieldExtract(ray.meta.x, 3,4+fb); };
uint lifetime(inout RayData ray, in uint a) { ray.meta.x = uint8_t(bitfieldInsert(ray.meta.x, a,3,4+fb)); return a; };
// 
uint kind(inout RayData ray) { return bitfieldExtract(ray.meta.x, 0, 3+fb); };
uint kind(inout RayData ray, in uint a) { ray.meta.x = uint8_t(bitfieldInsert(ray.meta.x, a,0,3+fb)); return a; };
//
bool finished(inout RayData ray) { return bool(bitfieldExtract(ray.meta.x,7,1+fb)); };
bool finished(inout RayData ray, in bool a) { ray.meta.x = uint8_t(bitfieldInsert(ray.meta.x,uint(a),7,1+fb)); return a; };
#endif


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


#endif

// 
//#include "./mathlib.hlsli"
