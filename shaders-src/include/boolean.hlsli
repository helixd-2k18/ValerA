#ifndef BOOLEAN_H
#define BOOLEAN_H

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

// 
#ifndef GLSL
bool3 lessThan(in float3 a, in float3 b) { return a < b; };
#endif

#endif
