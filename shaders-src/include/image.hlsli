#ifndef IMAGE_H
#define IMAGE_H

//
#ifndef GLSL
#define imageLoad(b, c) b[c]
//#define imageStore(b, c, f) (b[c] = f)
//#define texelFetch(b, c) b[c]
#define texelFetch(b, c, m) b[c]//b.Load(c,uint(m))
#define textureSample(b, s, c) b.Sample(s, c)
//#define textureSample(b, s, c, m) b.SampleLevel(s, c, m)
#define textureLodSample(b, s, c, m) b.SampleLevel(s, c, m)
#define nonuniformEXT(a) a

// 
#define fimage2D RWTexture2D<float>
void imageStore(in RWTexture2D<float> b, in int2 c, in float4 f) { b[c] = f; };
void imageStore(in RWTexture2D<float4> b, in int2 c, in float4 f) { b[c] = f; };
void imageStore(in RWTexture3D<float4> b, in int3 c, in float4 f) { b[c] = f; };
uint2 imageSize(in RWTexture2D<float4> tex) { uint2 size = uint2(0,0); tex.GetDimensions(size.x, size.y); return size; };
uint3 imageSize(in RWTexture3D<float4> tex) { uint3 size = uint3(0,0,0); tex.GetDimensions(size.x, size.y, size.z); return size; };
uint2 textureSize(in Texture2D<float4> tex, in int lod) { uint2 size = uint2(0,0); tex.GetDimensions(size.x, size.y); return size; };
uint3 textureSize(in Texture3D<float4> tex, in int lod) { uint3 size = uint3(0,0,0); tex.GetDimensions(size.x, size.y, size.z); return size; };
//uint2 textureSize(in Texture2D tex, in int lod) { uint2 size = uint2(0,0); return tex.GetDimensions(uint(lod), size.x, size.y); return size; };
//uint3 textureSize(in Texture3D tex, in int lod) { uint3 size = uint3(0,0,0); return tex.GetDimensions(uint(lod), size.x, size.y, size.z); return size; };

#else
#define fimage2D image2D
//
#define textureSample(b, s, c) texture(sampler2D(b, s), c)
#define textureLodSample(b, s, c, m) textureLod(sampler2D(b, s), c, m)
#endif

#define superImageLoad(image, texcoord) \
    float4(\
        imageLoad(image, int2(texcoord.x*4u+0u,texcoord.y)).x,\
        imageLoad(image, int2(texcoord.x*4u+1u,texcoord.y)).x,\
        imageLoad(image, int2(texcoord.x*4u+2u,texcoord.y)).x,\
        imageLoad(image, int2(texcoord.x*4u+3u,texcoord.y)).x\
    )

#define superImageStore(image, texcoord, fvalue) \
    imageStore(image, int2(texcoord.x*4u+0u,texcoord.y), fvalue.xxxx);\
    imageStore(image, int2(texcoord.x*4u+1u,texcoord.y), fvalue.yyyy);\
    imageStore(image, int2(texcoord.x*4u+2u,texcoord.y), fvalue.zzzz);\
    imageStore(image, int2(texcoord.x*4u+3u,texcoord.y), fvalue.wwww);

#ifdef GLSL 
#define atomicSuperImageAdd(image, texcoord, fvalue) \
    float4(\
        imageAtomicAdd(image, int2(texcoord.x*4u+0u,texcoord.y), fvalue.x),\
        imageAtomicAdd(image, int2(texcoord.x*4u+1u,texcoord.y), fvalue.y),\
        imageAtomicAdd(image, int2(texcoord.x*4u+2u,texcoord.y), fvalue.z),\
        imageAtomicAdd(image, int2(texcoord.x*4u+3u,texcoord.y), fvalue.w) \
    )

#define atomicSuperImageAdd3(image, texcoord, fvalue) \
    float4(\
        imageAtomicAdd(image, int2(texcoord.x*4u+0u,texcoord.y), fvalue.x),\
        imageAtomicAdd(image, int2(texcoord.x*4u+1u,texcoord.y), fvalue.y),\
        imageAtomicAdd(image, int2(texcoord.x*4u+2u,texcoord.y), fvalue.z),\
        imageAtomicExchange(image, int2(texcoord.x*4u+3u,texcoord.y), 1.f)\
    )

#endif


// 
float3 fromLinear(in float3 linearRGB) { return mix(float3(1.055)*pow(linearRGB, float3(1.0/2.4)) - float3(0.055), linearRGB * float3(12.92), lessThan(linearRGB, float3(0.0031308))); }
float3 toLinear(in float3 sRGB) { return mix(pow((sRGB + float3(0.055))/float3(1.055), float3(2.4)), sRGB/float3(12.92), lessThan(sRGB, float3(0.04045))); }
float4 fromLinear(in float4 linearRGB) { return float4(fromLinear(linearRGB.xyz), linearRGB.w); }
float4 toLinear(in float4 sRGB) { return float4(toLinear(sRGB.xyz), sRGB.w); }

// 
float2 flip(in float2 texcoord) { return float2(texcoord.x, 1.f - texcoord.y); };


#endif
