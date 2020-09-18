#include "../include/driver.hlsli"

#ifdef GLSL

// Right Oriented
layout (location = 0) flat out float4 uData;
layout (location = 1) flat out float4 fpt[3];
layout (location = 4) out float4 fBary;

// 
struct VS_INPUT 
{
    float3 iPosition;
    float2 iTexcoord;
    float3 iNormals;
    float4 iTangent;
    float4 iBinormal;
};

// 
struct PS_INPUT
{
    float4 Position;
    float4 fBary;
    float4 uData;
    float PointSize;
    float4 fpt[3];
};

#else

// 
struct VS_INPUT 
{
    float3 iPosition : LOCATION0;
    float2 iTexcoord : LOCATION1;
    float3 iNormals  : LOCATION2;
    float4 iTangent  : LOCATION3;
    float4 iBinormal : LOCATION4;
};

// 
struct PS_INPUT
{
    float4 Position      : SV_POSITION;
    float4 fBary         : POSITION0;
    nointerpolation float4 uData    : COLOR0;
    nointerpolation float PointSize : PSIZE0;
    nointerpolation float4 fpt[3]   : POSITION1;

};

#endif


// 
STATIC const float3 bary[3] = { float3(1.f,0.f,0.f), float3(0.f,1.f,0.f), float3(0.f,0.f,1.f) };
#ifdef GLSL
layout (points) in;
layout (triangle_strip, max_vertices = 3) out;
void main() 
#else
[maxvertexcount(3)]
PS_INPUT main(in triangle GS_INPUT inp[3], inout TriangleStream<TS_OUTPUT> OutputStream) 
#endif
{
#ifdef GLSL                        // InstanceID | GeometryID | PrimitiveID
    XHIT hit; hit.gIndices =    uint4(pushed.x,    pushed.y,    gl_PrimitiveIDIn, 0u);
    XTRI tri = geometrical(hit);

    for (uint i=0;i<3;i++) {
        fpt[i] = mul(getMT4x4(constants.projection), float4(mul(getMT3x4(constants.modelview), float4(tri.gPosition[i].xyz, 1.f)), 1.f));
        fpt[i] /= fpt[i].w;
        fpt[i].xyz = fpt[i].xyz * 0.5f + 0.5f;
        fpt[i].xy *= float2(textureSize(rasteredImagesRaw[0], 0));
    };

    for (uint i=0;i<3;i++) {
        gl_Position = mul(getMT4x4(constants.projection), float4(mul(getMT3x4(constants.modelview), float4(tri.gPosition[i].xyz, 1.f)), 1.f));
        gl_Position.y *= -1.f;
        fBary = float4(bary[i], 0.f);
        //fPosition = tri.gPosition[i];
        //fTexcoord = tri.gTexcoord[i];
        //fBarycent = float4(bary[i], 0.f);
        uData = uintBitsToFloat(hit.gIndices);
        EmitVertex();
    };
    EndPrimitive();
#endif
};
