#ifdef GLSL
#extension GL_EXT_ray_tracing          : require
#extension GL_EXT_ray_query            : require
#extension GL_ARB_post_depth_coverage  : require
#endif

// 
#include "../include/driver.hlsli"

// 
#ifdef GLSL
struct PS_INPUT
{
    float4 FragCoord;
    float4 fBary;
    float4 uData;
    float PointSize;
    float4 fpt[3];
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
layout (location = 0) flat in float4 uData;
layout (location = 1) flat in float4 fpt[3];
layout (location = 4) in float4 fBary;

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
    float4 fBary                 : POSITION2;
    nointerpolation float4 uData : COLOR0;
    float PointSize              : PSIZE0;
    float4 fpt[3]                : POSITION1;
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
float3 barycentric(in float3 p, in float3 d, in mat3x3 vc)
{
    const mat3x4 vcp = mat3x4(
        mul(getMT4x4(constants.projection), float4(vc[0], 1.f)),
        mul(getMT4x4(constants.projection), float4(vc[1], 1.f)),
        mul(getMT4x4(constants.projection), float4(vc[2], 1.f))
    );
    const float4 pp = mul(getMT4x4(constants.projection), float4(p, 1.f));

    float2 v0 = (vcp[2].xy/vcp[2].w) - (vcp[0].xy/vcp[0].w), v1 = (vcp[1].xy/vcp[1].w) - (vcp[0].xy/vcp[0].w), v2 = (pp.xy/pp.w) - (vcp[0].xy/vcp[0].w);
    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;

    // TODO: Perspective Correct
    return float3(u, w, v);
};

// 
//#define PIXEL_SCATTER
//#define BARY_TEST

// Так как НЕ прозрачные объекты не имеют толком прозрачностей, или не должны иметь, предпочитаем ранее тестирование...
#ifdef OPAQUE
layout ( early_fragment_tests ) in;
#endif

#ifdef GLSL 
void main() 
#else
PS_OUTPUT main(in PS_INPUT inp, in uint PrimitiveID : SV_PrimitiveID, float3 BaryWeights : SV_Barycentrics)
#endif
{   // TODO: Re-Interpolate for Randomized Center
#ifdef GLSL
    const float4 FragCoord = gl_FragCoord;

    PS_INPUT inp = { gl_FragCoord, fBary, uData, 0.f, fpt };
#endif

    const uint PrimitiveID = floatBitsToUint(inp.uData.z);
    const uint geometryInstanceID = floatBitsToUint(inp.uData.y);
    const uint globalInstanceID = floatBitsToUint(inp.uData.x);
    const uint geometrySetID = getGeometrySetID(instances[globalInstanceID]);

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

//#ifndef MatID
//#define MatID node.material
//#endif

    //
    packed = packUint2x16(uvec2(FragCoord.xy + 0.001f)), seed = uint2(packed, constants.rdata.x);

    // pixel scattering
#ifdef PIXEL_SCATTER
    float2 texc = floor(FragCoord.xy + 0.001f) + pRandom2(seed), size = float2(textureSize(rasteredImagesRaw[0], 0)); //+ pRandom2(seed);
#else
    float2 texc = FragCoord.xy, size = float2(textureSize(rasteredImagesRaw[0], 0));
#endif
    float3 origin = screen2camera(float3((texc/size)*2.f-1.f,0.001f));
    float3 target = screen2camera(float3((texc/size)*2.f-1.f,0.999f));
    float3 raydir = normalize(target - origin);

    // 
    float3 bary = barycentric(origin.xyz, raydir.xyz, mat3x3(inp.fpt[0].xyz, inp.fpt[1].xyz, inp.fpt[2].xyz));

    // Replacement for rasterization
    XHIT RPM; // currImages is Current Frame, prevImages is Previous Frame
    RPM.gBarycentric = inp.fBary;//float4(bary, 0.f);
    RPM.gIndices = floatBitsToUint(float4(inp.uData.xyz, 1.f));
    XGEO GEO = interpolate(RPM);
    XPOL MAT = materialize(RPM, GEO);

    // 
    const MaterialUnit unit = materials[MatID]; // NEW! 20.04.2020
    const float4 diffuseColor = toLinear(unit. diffuseTexture >= 0 ? textureSample(textures[nonuniformEXT(unit. diffuseTexture)], staticSamplers[2u], GEO.gTexcoord.xy) : unit.diffuse);

    // 
    PS_OUTPUT outp;
    outp.gPosition  = float4(0.f.xxxx);
    outp.oMaterial  = float4(0.f.xxxx);
    outp.oGeoIndice = float4(0.f.xxxx);
    outp.FragDepth  = 1.1f;

    // 
#ifdef BARY_TEST
    if (diffuseColor.w > 0.0001f && all(greaterThanEqual(bary, -0.001f.xxx))) 
#else
    if (diffuseColor.w > 0.0001f) 
#endif
    { // Only When Opaque!
        outp.gPosition = GEO.gPosition; // Save texcoord for Parallax Mapping with alpha channel
        outp.oMaterial = uintBitsToFloat(uint4(0u, 0u, 0u, floatBitsToUint(1.f)));
        outp.oGeoIndice = float4(inp.uData.xyz, 1.f);
        outp.oBarycent = float4(max(RPM.gBarycentric.xyz, 0.0001f.xxx), 1.f);
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
