#ifdef GLSL
#extension GL_EXT_ray_tracing          : require
#extension GL_EXT_ray_query            : require
#extension GL_ARB_post_depth_coverage  : require

#ifdef AMD_SOLUTION
#extension GL_AMD_shader_explicit_vertex_parameter : require
#define BARYCOORD float3(1.f-gl_BaryCoordSmoothAMD.x-gl_BaryCoordSmoothAMD.y,gl_BaryCoordSmoothAMD)
#else
#extension GL_NV_fragment_shader_barycentric : require
#define BARYCOORD gl_BaryCoordNV
#endif

#endif

// 
#include "../include/driver.hlsli"

// 
#ifdef GLSL
struct PS_INPUT
{
    float4 FragCoord;
    float4 fPosition;
    float4 fTexcoord;     
    float4 fBarycent;
    float4 uData;
    float PointSize;
};

// 
struct PS_OUTPUT {
     float4 oMaterial;
     float4 oGeoIndice; 
     float4 gPosition;
     float4 oBarycent;
     float FragDepth;
};

// 
layout (location = 0) in float4 fPosition;
layout (location = 1) in float4 fTexcoord;
layout (location = 2) in float4 fBarycent;
layout (location = 3) flat in float4 uData;

// 
layout (location = RS_MATERIAL) out float4 oMaterial;
layout (location = RS_INDICIES) out float4 oGeoIndice;
layout (location = RS_POSITION) out float4 gPosition;
layout (location = RS_BARYCENT) out float4 oBarycent;

#else

// 
struct PS_INPUT
{
    float4 FragCoord             : SV_POSITION;
    float4 fPosition             : POSITION0;
    float4 fTexcoord             : TEXCOORD0;     
    float4 fBarycent             : TEXCOORD1;
    nointerpolation float4 uData : COLOR0;
    float PointSize              : PSIZE0;
};

// 
struct PS_OUTPUT {
     float4 oMaterial   : SV_TARGET0;
     float4 oGeoIndice  : SV_TARGET1; 
     float4 gPosition   : SV_TARGET2;
     float4 oBarycent   : SV_TARGET3;
     float FragDepth    : SV_Depth;
};

#endif

// 
//layout ( early_fragment_tests ) in; // Reduce Lag Rate! (but transparency may broken!)
// Прозрачность с новой прошивкой починим! @RED21

//layout (location = RS_DIFFUSED) out float4 oDiffused;
//layout (location = RS_BARYCENT) out float4 oBarycent;

// TODO: FIX Conservative Rasterization! (i.e. add layer or virtualization)
// TODO: Triangle Edge Testing!
// TODO: Anti-Aliasing Support!
#ifdef GLSL 
void main() 
#else
PS_OUTPUT main(in PS_INPUT inp, in uint PrimitiveID : SV_PrimitiveID, float3 BaryWeights : SV_Barycentrics)
#endif
{   // TODO: Re-Interpolate for Randomized Center
#ifdef GLSL
    const float4 FragCoord = gl_FragCoord;
    const float3 BaryWeights = BARYCOORD;

    PS_INPUT inp = { gl_FragCoord, fPosition, fTexcoord, fBarycent, uData, 0.f };
#endif

    const uint PrimitiveID = floatBitsToUint(inp.uData.z);
    const uint geometryInstanceID = floatBitsToUint(inp.uData.y);
    const uint globalInstanceID = floatBitsToUint(inp.uData.x);
    const uint geometrySetID = getGeometrySetID(instances[globalInstanceID]);

    Interpolations interpol;
#ifdef GLSL
    interpol = interpolations[nonuniformEXT(geometrySetID)].data[geometryInstanceID];
#else
    interpol = interpolations[nonuniformEXT(geometrySetID)][geometryInstanceID];
#endif

    GeometryDesc node;
#ifdef GLSL
    node = geometries[nonuniformEXT(geometrySetID)].data[geometryInstanceID];
#else
    node = geometries[nonuniformEXT(geometrySetID)][geometryInstanceID];
#endif

    // By Geometry Data
    float3x4 matras = float3x4(float4(1.f,0.f.xxx),float4(0.f,1.f,0.f.xx),float4(0.f.xx,1.f,0.f));
    float3x4 matra4 = instances[nonuniformEXT(globalInstanceID)].transform;
    if (hasTransform(geometries[nonuniformEXT(geometrySetID)].data[geometryInstanceID])) { matras = node.transform; };

#ifndef MatID
#define MatID node.material
#endif

    const MaterialUnit unit = materials[MatID]; // NEW! 20.04.2020
    const float4 diffuseColor = toLinear(unit. diffuseTexture >= 0 ? textureSample(textures[nonuniformEXT(unit. diffuseTexture)], staticSamplers[2u], inp.fTexcoord.xy) : unit.diffuse);

    // 
    PS_OUTPUT outp;
    outp.gPosition  = float4(0.f.xxxx);
    outp.oMaterial  = float4(0.f.xxxx);
    outp.oGeoIndice = float4(0.f.xxxx);
    outp.FragDepth  = 1.1f;

    // 
    if (diffuseColor.w > 0.0001f) { // Only When Opaque!
        outp.gPosition = inp.fPosition; // Save texcoord for Parallax Mapping with alpha channel
        outp.oMaterial = uintBitsToFloat(uint4(0u, 0u, 0u, floatBitsToUint(1.f)));
        outp.oGeoIndice = uintBitsToFloat(uint4(globalInstanceID, geometryInstanceID, PrimitiveID, floatBitsToUint(1.f)));
        outp.oBarycent = float4(max(BaryWeights, 0.0001f.xxx), 1.f);
        outp.FragDepth = inp.FragCoord.z;
    };

#ifdef GLSL
    {
        //oOriginal = outp.oOriginal; // Needless
        gPosition = outp.gPosition;
        oMaterial = outp.oMaterial;
        oBarycent = outp.oBarycent;
        oGeoIndice = outp.oGeoIndice;
        gl_FragDepth = outp.FragDepth;
    };
#else
    return outp;
#endif

};
