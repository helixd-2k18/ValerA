//#define DRIVER_INCLUDE_H
#include "../include/driver.hlsli"

//#define FXAA_PC 1
//#define FXAA_GLSL_130 1
//#define FXAA_QUALITY_PRESET 39
//#include "./fxaa3_11.h"

#ifdef GLSL
layout ( location = 0 ) out float4 uFragColor;
layout ( location = 0 ) in float2 vcoord;

struct PSInput
{
    float4 position;
    float2 vcoord;
};

//
struct PSOutput 
{
    float4 uFragColor;
};

#else
// 
struct PSInput
{
    float4 position : SV_POSITION;
    float2 vcoord : COLOR;
};

//
struct PSOutput 
{
    float4 uFragColor : SV_TARGET0;
};
#endif


#ifndef FXAA_REDUCE_MIN
    #define FXAA_REDUCE_MIN   (1.0/ 128.0)
#endif
#ifndef FXAA_REDUCE_MUL
    #define FXAA_REDUCE_MUL   (1.0 / 8.0)
#endif
#ifndef FXAA_SPAN_MAX
    #define FXAA_SPAN_MAX     8.0
#endif


//optimized version for mobile, where dependent 
//texture reads can be a bottleneck
vec4 fxaa(vec2 fragCoord, vec2 resolution,
            vec2 v_rgbNW, vec2 v_rgbNE, 
            vec2 v_rgbSW, vec2 v_rgbSE, 
            vec2 v_rgbM) {
#define tex sampler2D(rasteredImagesRaw[RS_RENDERED], staticSamplers[1u])
    vec4 color;
    mediump vec2 inverseVP = vec2(1.0 / resolution.x, 1.0 / resolution.y);
    vec3 rgbNW = texture(tex, v_rgbNW).xyz;
    vec3 rgbNE = texture(tex, v_rgbNE).xyz;
    vec3 rgbSW = texture(tex, v_rgbSW).xyz;
    vec3 rgbSE = texture(tex, v_rgbSE).xyz;
    vec4 texColor = texture(tex, v_rgbM);
    vec3 rgbM  = texColor.xyz;
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    
    mediump vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
    
    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) *
                          (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
    
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
              max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
              dir * rcpDirMin)) * inverseVP;
    
    vec3 rgbA = 0.5 * (
        texture(tex, fragCoord * inverseVP + dir * (1.0 / 3.0 - 0.5)).xyz +
        texture(tex, fragCoord * inverseVP + dir * (2.0 / 3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (
        texture(tex, fragCoord * inverseVP + dir * -0.5).xyz +
        texture(tex, fragCoord * inverseVP + dir * 0.5).xyz);

    float lumaB = dot(rgbB, luma);
    if ((lumaB < lumaMin) || (lumaB > lumaMax))
        color = vec4(rgbA, texColor.a);
    else
        color = vec4(rgbB, texColor.a);
    return color;
};


// 
#ifdef GLSL
void main() 
#else
PSOutput main(in PSInput inp)
#endif
{ // TODO: explicit sampling 
#ifdef GLSL
    PSInput inp;
    inp.position = gl_FragCoord;
    inp.vcoord = vcoord;
#endif

    // Final Result Rendering
    PSOutput outp;
    outp.uFragColor = 0.f.xxxx;

    int2 size = textureSize(rasteredImagesRaw[RS_RENDERED], 0);//
    float2 nvi = 1.f / float2(size); //inp.vcoord
    
    //
    vec4 aac = fxaa(inp.vcoord*size, float2(size), 
        inp.vcoord + float2(-nvi.x, -nvi.y), inp.vcoord + float2(nvi.x, -nvi.y),
        inp.vcoord + float2(-nvi.x,  nvi.y), inp.vcoord + float2(nvi.x,  nvi.y),
        inp.vcoord
    );

    outp.uFragColor = fromLinear(aac);

#ifdef GLSL
    uFragColor = outp.uFragColor;
#else
    return outp;
#endif
};
