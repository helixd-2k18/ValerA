#include "../include/driver.hlsli"

#ifdef GLSL

// Right Oriented
layout (location = 0) out float4 fPosition;
layout (location = 1) out float4 fTexcoord;
layout (location = 2) out float4 fBarycent;
layout (location = 3) flat out float4 uData;

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
    float4 fPosition;
    float4 fTexcoord;     
    float4 fBarycent;
    float4 uData;
    float PointSize;
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
    float4 Position              : SV_POSITION;
               float4 fPosition  : POSITION0;
               float4 fTexcoord  : TEXCOORD0;     
               float4 fBarycent  : TEXCOORD1;
    nointerpolation float4 uData : COLOR0;
    float PointSize              : PSIZE0;
};

#endif


// 
STATIC const float3 bary[3] = { float3(1.f,0.f,0.f), float3(0.f,1.f,0.f), float3(0.f,0.f,1.f) };
#ifdef GLSL
layout (triangles) in;
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
        gl_Position = mul(getMT4x4(constants.projection), float4(mul(getMT3x4(constants.modelview), float4(tri.gPosition[i].xyz, 1.f)), 1.f));
        fPosition = tri.gPosition[i];
        fTexcoord = tri.gTexcoord[i];
        fBarycent = float4(bary[i], 0.f);
        uData = uintBitsToFloat(hit.gIndices);
        EmitVertex();
    };
    EndPrimitive();
#endif
};
