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
float3 barycentric(in float3 P, in float3 A, in float3 B, in float3 C) {

    // Compute vectors        
    float3 v0 = C - A;
    float3 v1 = B - A;
    float3 v2 = P - A;

    // Compute dot products
    float dot00 = dot(v0, v0);
    float dot01 = dot(v0, v1);
    float dot02 = dot(v0, v2);
    float dot11 = dot(v1, v1);
    float dot12 = dot(v1, v2);

    // Compute barycentric coordinates
    float invDenom = 1.f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // 
    return float3(1.f - u - v, v, u);
};


// Так как НЕ прозрачные объекты не имеют толком прозрачностей, или не должны иметь, предпочитаем ранее тестирование...
#ifdef OPAQUE
//layout ( early_fragment_tests ) in;
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

    // 
    float2 texc = FragCoord.xy; //+ pRandom2(seed);
    float3 bary = fBary.xyz;//barycentric(FragCoord.xyz, inp.fpt[0].xyz, inp.fpt[1].xyz, inp.fpt[2].xyz);

    // Replacement for rasterization
    XHIT RPM; // currImages is Current Frame, prevImages is Previous Frame
    RPM.gBarycentric = float4(bary, 0.f);
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
    if (diffuseColor.w > 0.0001f && all(greaterThanEqual(bary, 0.f.xxx))) { // Only When Opaque!
        outp.gPosition = GEO.gPosition; // Save texcoord for Parallax Mapping with alpha channel
        outp.oMaterial = uintBitsToFloat(uint4(0u, 0u, 0u, floatBitsToUint(1.f)));
        outp.oGeoIndice = float4(inp.uData.xyz, 1.f);
        outp.oBarycent = float4(max(bary.xyz, 0.0001f.xxx), 1.f);
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
