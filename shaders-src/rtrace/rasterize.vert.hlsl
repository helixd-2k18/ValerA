#define DRIVER_INCLUDE_H
#include "../include/driver.hlsli"

#ifndef GLSL

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
void main() 
#else
PS_INPUT main(in VS_INPUT inp, in uint InstanceIndex : SV_InstanceID, in uint VertexIndex : SV_VertexID)
#endif
{
#ifdef GLSL
    gl_Position = vec4(0.f.xxx, 1.f);
#endif
};
