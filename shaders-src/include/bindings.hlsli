#ifndef BINDINGS_H
#define BINDINGS_H

// 
#include "./driver.hlsli"
#include "./mathlib.hlsli"


// Ray-Tracing Data (With resampling output support!)
#define IW_INDIRECT 0  // Indrect Diffuse
#define IW_REFLECLR 1  // Previous Frame Reflection
#define IW_TRANSPAR 2  // Semi-Transparent Mixing
#define IW_GEONORML 3  // Geometry Normals
#define IW_MAPNORML 4  // Mapped Normals
#define IW_SHADOWCL 5  // Shadow Accumulation
#define IW_ORIGINAL 6  // 
#define IW_ADAPTIVE 7  // 
#define IW_INDICIES 8  // 
#define IW_POSITION 9  // Ray-Traced Position (for resampling)
#define IW_RENDERED 10 // 
#define IW_BARYCENT 11

// From Rasterization Phase!
#define RS_MATERIAL 0
#define RS_INDICIES 1
#define RS_POSITION 2
#define RS_BARYCENT 3

// For OptiX
#define RS_INDIRECT 4
#define RS_ALBEDOBF 5
#define RS_NORMALED 6
#define RS_RENDERED 7
#define RS_REFLECLR 8


#ifdef GLSL
layout (binding = 0, set = 0, scalar) uniform Matrices {
    layout(column_major) float4x4 projection;    
    layout(column_major) float4x4 projectionInv; 
    layout(column_major) float3x4 modelview;   
    layout(column_major) float3x4 modelviewInv;
    layout(column_major) float3x4 modelviewPrev;   
    layout(column_major) float3x4 modelviewPrevInv; 
    uint4 mdata;        // mesh mutation or modification data
    //uint2 tdata, rdata; // first for time, second for randoms
    uint2 tdata;
    uint2 rdata;
} constants;

// 
layout (binding = 0, set = 1, scalar) readonly buffer MeshData { uint8_t data[]; } mesh0[];
//layout (binding = 0, set = 1, r8ui) readonly uniform uimageBuffer mesh0[];
layout (binding = 0, set = 2, scalar) readonly buffer Bindings   { Binding   bindings[]; };
layout (binding = 0, set = 3, scalar) readonly buffer Attributes { Attribute attributes[]; };
//layout (binding = 0, set = 2, scalar) readonly buffer Bindings   { Binding   data[8u]; } bindings  [];
//layout (binding = 0, set = 3, scalar) readonly buffer Attributes { Attribute data[8u]; } attributes[];

// Deferred and Rasterization Set
layout (binding = 0, set = 4,    r32f) uniform image2D prevImages[12u]; // 4x wider
layout (binding = 1, set = 4,    r32f) uniform image2D currImages[12u]; // 4x wider
layout (binding = 2, set = 4,    r32f) uniform image2D resampleImages[12u]; 
layout (binding = 3, set = 4, rgba32f) uniform image2D rasteredImages[12u]; // used by rasterization
layout (binding = 4, set = 4         ) uniform sampler staticSamplers[8u];
layout (binding = 6, set = 4         ) uniform texture2D rasteredImagesRaw[12u]; 

// Material Set
layout (binding = 0, set = 5, scalar) readonly buffer Materials { MaterialUnit materials[]; };
layout (binding = 0, set = 6) uniform texture2D textures[];
layout (binding = 0, set = 7) uniform texture2D samplers[];
// TODO: Samplers Set

layout (binding = 0, set = 8, scalar) readonly buffer Geometries { GeometryDesc data[]; } geometries[];


// 
#ifdef ENABLE_AS
layout (binding = 0, set = 10 ) uniform accelerationStructureEXT Scene;
#endif

// TODO: Ray Tracing Structures


// 
layout (binding = 0, set = 15) uniform texture2D background;
layout (binding = 0, set = 16, scalar) readonly buffer Instances { RTInstance instances[]; };

// 
layout (binding = 0, set = 11, scalar) buffer Counters { uint counters[5u]; };
layout (binding = 0, set = 12, scalar) buffer RayDatas { RayData data[]; } rays[2u];
layout (binding = 0, set = 13, scalar) buffer HitDatas { HitData data[]; } hits[1u];

// 
layout (push_constant) uniform pushConstants { uint4 pushed; };

//
#define textureSample(b, s, c) texture(sampler2D(b, s), c)
#define textureLodSample(b, s, c, m) textureLod(sampler2D(b, s), c, m)
//#define textureSample(b, s, c, m) textureLod(sampler2D(b, s), c, m)
#else

struct Matrices {
    column_major float4x4 projection;    
    column_major float4x4 projectionInv; 
    column_major float3x4 modelview;   
    column_major float3x4 modelviewInv;
    column_major float3x4 modelviewPrev;   
    column_major float3x4 modelviewPrevInv; 
    uint4 mdata;        // mesh mutation or modification data
    //uint2 tdata, rdata; // first for time, second for randoms
    uint2 tdata;
    uint2 rdata;
};

struct DrawInfo {
    uint4 data;
};

// 
ConstantBuffer<Matrices> constants : register(b0, space0);
RWByteAddressBuffer mesh0[] : register(u0, space1);
RWStructuredBuffer<Binding> bindings[] : register(t0, space2);
RWStructuredBuffer<Attribute> attributes[] : register(t0, space3);

// 
RWStructuredBuffer<GeometryDesc> geometries[] : register(t0, space8);
RWStructuredBuffer<Interpolations> interpolations[] : register(t0, space9);

// 
#ifdef ENABLE_AS
RaytracingAccelerationStructure Scene : register(t0, space10);
#endif

// Deferred and Rasterization Set
RWTexture2D<float4> prevImages[12u]    : register(u0 , space4); // Pre-resampled buffers
RWTexture2D<float4> currImages[12u]    : register(u12, space4); // Used by rasterization
RWTexture2D<float4> resampleImages[8u] : register(u24, space4);
RWTexture2D<float4> rasteredImages[8u] : register(u32, space4);
SamplerState        staticSamplers[8u] : register(t0, space4);

// 
RWStructuredBuffer<MaterialUnit> materials : register(u0, space5);
Texture2D<float4> textures[] : register(t0, space6);

// TODO: Ray Tracing Structures


//
Texture2D<float4> background : register(t0, space15);
ConstantBuffer<DrawInfo> drawInfo : register(b0, space20);
#endif



// 
struct RCData {
    uint4 udata;
    float4 fdata;
};

// BUT DEFAULT OVERRIDEN!
STATIC int2 launchSize = int2(1600, 1200);

// 
struct XPOL {
     float4  diffuseColor;
     float4 emissionColor;
     float4  normalsColor;
     float4 specularColor;

     float4 mapNormal; float4 txcmid;
};

struct XGEO {
    float4 oPosition; float4 gPosition; float4 gTangent; float4 gBinormal; float4 gNormal; float4 gTexcoord;
};

struct XHIT {
     float4 origin; float4 direct;
     
     float4 gBarycentric;
    uint4 gIndices;
};

struct CHIT {
     float4 gBarycentric;
    uint4 gIndices;
};

// 
struct XTRI {
    float3x4 oPosition;
    float3x4 gPosition;
    float3x4 gTangent; 
    float3x4 gBinormal; 
    float3x4 gNormal; 
    float3x4 gTexcoord;
};


// 
STATIC uint packed = 0u;
STATIC uint2 seed = uint2(0u.xx);


// 
uint getGeometrySetID(in RTInstance instance){
    return bitfieldExtract(instance.instance_mask, 0, 24); // only hack method support
};

uint getGeometrySetID(in uint instance){
    return bitfieldExtract(instances[instance].instance_mask, 0, 24); // only hack method support
};

// Deprecated
#define getMeshID getGeometrySetID

// RESERVED FOR OTHER OPERATIONS
float3 refractive(in float3 dir) {
    return dir;
};


// Per System Specified
#ifdef GLSL
uint8_t load_u8(in uint offset, in uint binding) {
    return mesh0[nonuniformEXT(binding)].data[offset]; // 03.08.2020 - here is struked...
};

uint16_t load_u16(in uint offset, in uint binding) {
    return pack16(u8vec2(load_u8(offset,binding),load_u8(offset+1u,binding)));
};

uint load_u32(in uint offset, in uint binding) {
    return pack32(u16vec2(load_u16(offset,binding),load_u16(offset+2u,binding)));
};
#else
uint load_u32(in uint offset, in uint binding) {
    return mesh0[binding].Load(int(offset)).x;
};
#endif

// Made implicit due some not-valid GLTF (disable overlap formats)
uint formatStride(in uint format) {
    // F32_T
    if (format >= 107 && format <= 109) { return 16u; } else 
    if (format >= 104 && format <= 106) { return 12u; } else
    if (format >= 101 && format <= 103) { return 8u; } else
    if (format >= 98  && format <= 100) { return 4u; };

    // F16_T
    // TODO: 

    return 4u;
};

uint predicate(in uint stride, in uint format){
    return stride > 0u ? stride : format;
};


// TODO: Add Uint16_t, uint, Float16_t Support
float4 get_float4(in uint idx, in uint loc) {
    Attribute attrib = attributes[loc];
    Binding  binding = bindings[attrib.binding];

    // 
    uint stride = predicate(binding.stride, formatStride(attrib.format));
    uint boffset = stride * idx + attrib.offset;
    float4 vec = float4(0.f.xxxx);
    
    // 
    if (stride >  0u) vec[0] = uintBitsToFloat(load_u32(boffset +  0u, binding.binding));
    if (stride >  4u) vec[1] = uintBitsToFloat(load_u32(boffset +  4u, binding.binding));
    if (stride >  8u) vec[2] = uintBitsToFloat(load_u32(boffset +  8u, binding.binding));
    if (stride > 12u) vec[3] = uintBitsToFloat(load_u32(boffset + 12u, binding.binding));
    
    // 
    return vec;
};

float3 world2screen(in float3 origin){
    return divW(mul(constants.projection, float4(mul(constants.modelview, float4(origin,1.f)), 1.f)));
};

float3 screen2world(in float3 origin){
    return mul(constants.modelviewInv, float4(divW(mul(constants.projectionInv, float4(origin,1.f))), 1.f));
};

float3 screen2camera(in float3 origin){
    return divW(mul(constants.projectionInv, float4(origin,1.f)));
};

float3 world2screen(in float4 origin){
    return divW(mul(constants.projection, float4(mul(constants.modelview, origin), 1.f)));
};

float3 screen2world(in float4 origin){
    return mul(constants.modelviewInv, float4(divW(mul(constants.projectionInv, origin)), 1.f));
};


float3x4 triangled(in uint3 indices, in uint loc){
    return float3x4(
        get_float4(indices[0],loc),
        get_float4(indices[1],loc),
        get_float4(indices[2],loc)
    );
};

float4 triangulate(in uint3 indices, in uint loc, in float3 barycenter){
    return mul(barycenter, triangled(indices, loc));
};


// Some Settings
#define DIFFUSE_COLOR diffuseColor.xyz
#define EMISSION_COLOR emissionColor.xyz

float4 gSkyShader(in float3 raydir, in float3 origin) {
    return float4(textureSample(background, staticSamplers[3u], flip(lcts(raydir.xyz))).xyz, 1.f);
};

// 
float  staticRandom () { SCLOCK += 1; return floatConstruct(hash(uint4(SCLOCK,0u, constants.rdata.xy))); }
float2 staticRandom2() { SCLOCK += 1; return  halfConstruct(hash(uint4(SCLOCK,0u, constants.rdata.xy))); }

// 
XTRI geometrical(in XHIT hit) { // By Geometry Data
    const uint geometryInstanceID = hit.gIndices.y;
    const uint globalInstanceID = hit.gIndices.x;
    const uint primitiveID = hit.gIndices.z;
    const uint geometrySetID  = getGeometrySetID(instances[globalInstanceID]);
    const float3 baryCoord = hit.gBarycentric.xyz;

    // 
    const GeometryDesc node = 
#ifdef GLSL
    geometries[nonuniformEXT(geometrySetID)].data[geometryInstanceID];
#else
    geometries[nonuniformEXT(geometrySetID)][geometryInstanceID];
#endif

    // By Geometry Data
    float3x4 matras = float3x4(float4(1.f,0.f.xxx),float4(0.f,1.f,0.f.xx),float4(0.f.xx,1.f,0.f));
    float3x4 matra4 = instances[nonuniformEXT(globalInstanceID)].transform;
    if (hasTransform(node)) { matras = node.transform; };

    // Native Normal Transform
    const float3x3 normalTransform = inverse(transpose(regen3(matras)));
    const float3x3 normInTransform = inverse(transpose(regen3(matra4)));

    // 
    uint3 idx3 = uint3(primitiveID*3u+0u+node.firstVertex,primitiveID*3u+1u+node.firstVertex,primitiveID*3u+2u+node.firstVertex);
    if (node.indexType == 1000265000) { idx3 = uint3(load_u8 (idx3.x<<0, node.indexBufferView),load_u8 (idx3.y<<0, node.indexBufferView),load_u8 (idx3.z<<0, node.indexBufferView)); };
    if (node.indexType == 0)          { idx3 = uint3(load_u16(idx3.x<<1, node.indexBufferView),load_u16(idx3.y<<1, node.indexBufferView),load_u16(idx3.z<<1, node.indexBufferView)); };
    if (node.indexType == 1)          { idx3 = uint3(load_u32(idx3.x<<2, node.indexBufferView),load_u32(idx3.y<<2, node.indexBufferView),load_u32(idx3.z<<2, node.indexBufferView)); };

    // 
    XTRI geometry;
    geometry.gPosition  = triangled(idx3, node.vertexAttribute);
    geometry.gTexcoord  = triangled(idx3, node.attributes[0u]);
    geometry.gNormal    = triangled(idx3, node.attributes[1u]);
    geometry.gTangent   = triangled(idx3, node.attributes[2u]);
    geometry.gBinormal  = triangled(idx3, node.attributes[3u]);

    // 
    for (uint32_t i=0u;i<3u;i++) {
        geometry.oPosition[i] = geometry.gPosition[i];
        geometry.gPosition[i] = float4(mul(matra4, float4(mul(matras, float4(geometry.oPosition[i].xyz, 1.f)), 1.f)), 1.f);
    };

    // 
    const float4 dp1 = geometry.gPosition[1] - geometry.gPosition[0], dp2 = geometry.gPosition[2] - geometry.gPosition[0];
    const float4 tx1 = geometry.gTexcoord[1] - geometry.gTexcoord[0], tx2 = geometry.gTexcoord[2] - geometry.gTexcoord[0];
    const float3 normal = normalize(cross(dp1.xyz, dp2.xyz));

    // 
    const float coef = 1.f / (tx1.x * tx2.y - tx2.x * tx1.y);
    const float3 tangent = (dp1.xyz * tx2.yyy - dp2.xyz * tx1.yyy) * coef;
    const float3 binorml = (dp1.xyz * tx2.xxx - dp2.xyz * tx1.xxx) * coef;

    // 
    for (uint32_t i=0u;i<3u;i++) { // 
        geometry.gNormal[i] = float4(geometry.gNormal[i].xyz, 0.f);
        geometry.gTangent[i] = float4(geometry.gTangent[i].xyz, 0.f);
        geometry.gBinormal[i] = float4(geometry.gBinormal[i].xyz, 0.f);

        // 
        if (!hasNormal (node) || dot(abs(geometry.gNormal [i].xyz), 1.f.xxx) < 0.0001f) { geometry.gNormal[i]  = float4(normal, 0.f); };
        if (!hasTangent(node) || dot(abs(geometry.gTangent[i].xyz), 1.f.xxx) < 0.0001f) { 
            geometry.gTangent[i].xyz  =  (tangent - dot(geometry.gNormal[i].xyz,tangent.xyz)*geometry.gNormal[i].xyz);
            geometry.gBinormal[i].xyz = -(binorml - dot(geometry.gNormal[i].xyz,binorml.xyz)*geometry.gNormal[i].xyz);
        } else {
            geometry.gBinormal[i].xyz = normalize(cross(geometry.gNormal[i].xyz, geometry.gTangent[i].xyz));
        };

        // 
        geometry.gNormal[i].xyz = mul(normInTransform, mul(normalTransform, geometry.gNormal[i].xyz));
        geometry.gTangent[i].xyz = mul(normInTransform, mul(normalTransform, geometry.gTangent[i].xyz));
        geometry.gBinormal[i].xyz = mul(normInTransform, mul(normalTransform, geometry.gBinormal[i].xyz));

        // 
        geometry.gNormal[i].xyz = normalize(geometry.gNormal[i].xyz);
        geometry.gTangent[i].xyz = normalize(geometry.gTangent[i].xyz);
        geometry.gBinormal[i].xyz = normalize(geometry.gBinormal[i].xyz);
    };

    // 
    return geometry;
};

// 
XGEO interpolate(in XHIT hit) { // By Geometry Data
    const float3 baryCoord = hit.gBarycentric.xyz;

    // 
    XTRI geometryMat = geometrical(hit);

    // 
    XGEO geometry;
    geometry.oPosition  = mul(geometryMat.oPosition, baryCoord);
    geometry.gPosition  = mul(geometryMat.gPosition, baryCoord);
    geometry.gTexcoord  = mul(geometryMat.gTexcoord, baryCoord);
    geometry.gNormal    = mul(geometryMat.gNormal, baryCoord);
    geometry.gTangent   = mul(geometryMat.gTangent, baryCoord);
    geometry.gBinormal  = mul(geometryMat.gBinormal, baryCoord);

    // 
    return geometry;
};

// 
XPOL materialize(in XHIT hit, inout XGEO geo) { // 
#define MatID node.material

    XPOL material;
    material. diffuseColor = float4(0.f.xxx, 1.f.x);
    material.emissionColor = gSkyShader(hit.direct.xyz, hit.origin.xyz);
    material. normalsColor = float4(0.5f,0.5f,1.f,1.f);
    material.specularColor = float4(0.f.xxx,0.f.x); // TODO: Correct Specular Initial
    material.mapNormal = geo.gNormal;
    material.txcmid = float4(uintBitsToFloat(packUnorm2x16(0.f.xx)), 0.f, 0.f, 0.f); // 

    // 
    const uint geometryInstanceID = hit.gIndices.y;
    const uint globalInstanceID = hit.gIndices.x;
    const uint primitiveID = hit.gIndices.z;
    const uint geometrySetID = getGeometrySetID(instances[globalInstanceID]);

    const GeometryDesc node = 
#ifdef GLSL
    geometries[nonuniformEXT(geometrySetID)].data[geometryInstanceID];
#else
    geometries[nonuniformEXT(geometrySetID)][geometryInstanceID];
#endif

    const MaterialUnit unit = materials[MatID]; // NEW! 20.04.2020
    const float2 gTexcoord = geo.gTexcoord.xy;
    //const float3 gNormal = geo.gNormal.xyz;

    // 
    if (hit.gBarycentric.w < 9999.f) {
        material. diffuseColor = toLinear(unit. diffuseTexture >= 0 ? textureSample(textures[nonuniformEXT(unit. diffuseTexture)],staticSamplers[2u],gTexcoord.xy) : unit.diffuse);
        material.emissionColor = toLinear(unit.emissionTexture >= 0 ? textureSample(textures[nonuniformEXT(unit.emissionTexture)],staticSamplers[2u],gTexcoord.xy) : unit.emission);
        material. normalsColor = unit. normalsTexture >= 0 ? textureSample(textures[nonuniformEXT(unit. normalsTexture)],staticSamplers[2u],gTexcoord.xy) : unit.normals;
        material.specularColor = unit.specularTexture >= 0 ? textureSample(textures[nonuniformEXT(unit.specularTexture)],staticSamplers[2u],gTexcoord.xy) : unit.specular;

        // Mapping
        material.mapNormal = float4(normalize(mul(normalize(material.normalsColor.xyz * 2.f - 1.f), float3x3(geo.gTangent.xyz, geo.gBinormal.xyz, geo.gNormal.xyz))), 1.f);

        // Use real origin
        material.txcmid = float4(uintBitsToFloat(packUnorm2x16(fract(geo.gTexcoord.xy))), uintBitsToFloat(MatID), 1.f, 0.f); // 
    };

    // 
    return material;
};

// 
XHIT rasterize(in float3 origin, in float3 raydir, float maxT, bool scatterTransparency, float threshold) {
    uint I = 0, R = 0; float lastMax = maxT, lastMin = 0.001f; float3 forigin = origin + raydir.xyz * lastMin;

    // 
    float fullLength = 0.f;
    bool restart = false;

    // 
    XHIT processing, confirmed;
    processing.origin = float4(origin.xyz + raydir.xyz * lastMax, 1.f);
    processing.direct = float4(raydir.xyz, 0.f);
    processing.gIndices = uint4(0u.xxxx);
    processing.gBarycentric = float4(0.f.xxx, lastMax);
    confirmed = processing;

    // 
    float3 sslr = world2screen(origin);
    const int2 tsize = imageSize(rasteredImages[RS_MATERIAL]);
    const int2 samplep = int2((sslr.xy*0.5f+0.5f) * imageSize(rasteredImages[RS_INDICIES]));
    const uint4 indices  = floatBitsToUint(imageLoad(rasteredImages[RS_INDICIES], samplep));

    // 
    const float3 baryCoord = imageLoad(rasteredImages[RS_BARYCENT], samplep).xyz;
    const bool isSkybox = dot(baryCoord.yz,1.f.xx)<=0.f; //uintBitsToFloat(datapass.z) <= 0.99f;
    const uint primitiveID = indices.z;
    const uint geometryInstanceID = indices.y;
    const uint globalInstanceID = indices.x;
    const uint geometrySetID = getGeometrySetID(instances[globalInstanceID]);

    // 
    if (!isSkybox) { // Only When Opaque!
        processing.direct = float4(raydir.xyz, 0.f);
        processing.origin = imageLoad(rasteredImages[RS_POSITION], samplep);

        // Interpolate In Ray-Tracing
        processing.gIndices = indices;
        processing.gBarycentric = float4(baryCoord, distance(processing.origin.xyz, origin.xyz));

        // Interpolate In Ray-Tracing
        XGEO geometry = interpolate(processing);
        XPOL material = materialize(processing, geometry);

        // 
        //processing.origin = geometry.gPosition;

        // TODO: rasterization direct diffuse access
        if (material.diffuseColor.w > 0.0001f) { // Only When Opaque!
            confirmed = processing;
        };
    };

    return confirmed;
};

#endif
