#ifndef RAYTRACING_H
#define RAYTRACING_H

#include "./driver.hlsli"
#include "./mathlib.hlsli"
#include "./bindings.hlsli"

// Needs 1000$ for fix BROKEN ray query...
STATIC const uint MAX_ITERATION = 16u;


#ifndef GLSL
#define rayQueryGetIntersectionInstanceCustomIndexEXT(q, B) (B?q.CommittedInstanceID():q.CandidateInstanceID())
#define rayQueryGetIntersectionGeometryIndexEXT(q, B) (B?q.CommittedGeometryIndex():q.CandidateGeometryIndex())
#define rayQueryGetIntersectionInstanceIdEXT(q, B) (B?q.CommittedInstanceIndex():q.CandidateInstanceIndex())
#define rayQueryGetIntersectionBarycentricsEXT(q, B) (B?q.CommittedTriangleBarycentrics():q.CandidateTriangleBarycentrics())
#define rayQueryGetIntersectionPrimitiveIndexEXT(q, B) (B?q.CommittedPrimitiveIndex():q.CandidatePrimitiveIndex())
#define rayQueryGetIntersectionTEXT(q, B) (B?q.CommittedRayT():q.CandidateTriangleRayT())
#define rayQueryProceedEXT(q) q.Proceed()
#define rayQueryGetIntersectionTypeEXT(q, B) (B?q.CommittedStatus():COMMITTED_NOTHING)
#define rayQueryConfirmIntersectionEXT(q) q.CommitNonOpaqueTriangleHit()
#define rayQueryTerminateEXT(q) q.Abort()

// 
void rayQueryInitializeEXT(in RayQuery<RAY_FLAG_CULL_BACK_FACING_TRIANGLES> rayQuery, in uint flags, in uint mask, in float3 origin, in float minT, in float3 direct, in float maxT) {
    RayDesc desc;
    desc.Origin = origin;
    desc.TMin = minT;
    desc.Direction = direct;
    desc.TMax = maxT;

    // 
    rayQuery.TraceRayInline(Scene, flags, mask, desc);
};
#else
#define RAY_FLAG_FORCE_OPAQUE gl_RayFlagsOpaqueEXT
#define RAY_FLAG_CULL_BACK_FACING_TRIANGLES gl_RayFlagsCullBackFacingTrianglesEXT
#define COMMITTED_NOTHING gl_RayQueryCommittedIntersectionNoneEXT

void rayQueryInitializeEXT(in rayQueryEXT rayQuery, in uint flags, in lowp uint mask, in float3 origin, in float minT, in float3 direct, in float maxT) {
    rayQueryInitializeEXT(rayQuery, Scene, flags, mask, origin, minT, direct, maxT);
};
#endif

// Ray Query Broken In Latest Driver... 
XHIT traceRays(in float3 origin, in float3 raydir, float maxT, bool scatterTransparency, float threshold) {
    uint I = 0, R = 0; float lastMax = maxT, lastMin = 0.001f, fullLength = 0.f; float3 forigin = origin + raydir.xyz * lastMin, sorigin = forigin;

    // 
    XHIT processing, confirmed;
    processing.origin = float4(origin.xyz + raydir.xyz * lastMax, 1.f);
    processing.direct = float4(raydir.xyz, 0.f);
    processing.gIndices = uint4(0u.xxxx);
    processing.gBarycentric = float4(0.f.xxx, lastMax);
    confirmed = processing;

    // 
    bool restart = true, opaque = false;
    while((R++) < 1u && restart) { restart = false; // restart needs for transparency (after every resolve)
        float lastMax = (maxT - fullLength); float3 lastOrigin = forigin;//raydir * fullLength + sorigin;

#ifdef GLSL
        rayQueryEXT rayQuery;
#else
        RayQuery<RAY_FLAG_CULL_BACK_FACING_TRIANGLES> rayQuery;
#endif

        rayQueryInitializeEXT(rayQuery, RAY_FLAG_CULL_BACK_FACING_TRIANGLES, 0xFF, lastOrigin, lastMin, raydir, lastMax);

        // BROKEN `rayQueryProceedEXT`
        bool proceed = false;
        I = 0; while((I++) < MAX_ITERATION && (proceed = rayQueryProceedEXT(rayQuery))) { // 
            uint nodeMeshID = rayQueryGetIntersectionInstanceCustomIndexEXT(rayQuery, false); // Mesh ID from Node Mesh List (because indexing)
            uint geometryInstanceID = rayQueryGetIntersectionGeometryIndexEXT(rayQuery, false); // TODO: Using In Ray Tracing (and Query) shaders!
            uint globalInstanceID = rayQueryGetIntersectionInstanceIdEXT(rayQuery, false);
            float2 baryCoord = rayQueryGetIntersectionBarycentricsEXT(rayQuery, false);
            uint primitiveID = rayQueryGetIntersectionPrimitiveIndexEXT(rayQuery, false); 
            //float3 origin = rayQueryGetIntersectionObjectRayOriginEXT(rayQuery, false);
            float tHit = rayQueryGetIntersectionTEXT(rayQuery, false);

            // 
            {   //lastOrigin = raydir*(lastMax = tHit) + forigin;
                XHIT hit = processing;
                hit.gIndices = uint4(globalInstanceID, geometryInstanceID, primitiveID, 0u);
                hit.gBarycentric.xyz = float3(1.f-baryCoord.x-baryCoord.y,baryCoord);
                hit.origin = float4(raydir*(hit.gBarycentric.w = (fullLength + tHit)) + sorigin, 1.f);

                // Interpolate In Ray-Tracing
                XGEO geometry = interpolate(hit);
                XPOL material = materialize(hit, geometry);

                // confirm that hit 
                if (material.diffuseColor.w > (scatterTransparency ? random(seed) : threshold)) { // Only When Opaque!
                    rayQueryConfirmIntersectionEXT(rayQuery); // override hit hit
                };
            };
        };

        // 
        processing = confirmed; lastMax = (maxT - fullLength); lastOrigin = raydir*maxT + sorigin; opaque = false;
        if (!proceed) { // Attemp to fix Broken Ray Query
            if (rayQueryGetIntersectionTypeEXT(rayQuery, true) != COMMITTED_NOTHING) 
            {
                uint nodeMeshID = rayQueryGetIntersectionInstanceCustomIndexEXT(rayQuery, true); // Mesh ID from Node Mesh List (because indexing)
                uint geometryInstanceID = rayQueryGetIntersectionGeometryIndexEXT(rayQuery, true); // TODO: Using In Ray Tracing (and Query) shaders!
                uint globalInstanceID = rayQueryGetIntersectionInstanceIdEXT(rayQuery, true);
                float2 baryCoord = rayQueryGetIntersectionBarycentricsEXT(rayQuery, true);
                uint primitiveID = rayQueryGetIntersectionPrimitiveIndexEXT(rayQuery, true); 
                //float3 origin = rayQueryGetIntersectionObjectRayOriginEXT(rayQuery, true);
                float tHit = rayQueryGetIntersectionTEXT(rayQuery, true);

                // 
                if (tHit < lastMax) { lastOrigin = raydir*(lastMax=tHit) + forigin;
                    processing.gIndices = uint4(globalInstanceID, geometryInstanceID, primitiveID, 0u);
                    processing.gBarycentric.xyz = float3(1.f-baryCoord.x-baryCoord.y,baryCoord);
                    processing.origin.xyz = (forigin = (raydir*(processing.gBarycentric.w = (fullLength += tHit)) + sorigin));

                    // Interpolate In Ray-Tracing
                    XGEO geometry = interpolate(processing);
                    XPOL material = materialize(processing, geometry);

                    // 
                    //forigin += faceforward(geometry.gNormal.xyz, -raydir.xyz, geometry.gNormal.xyz) * lastMin + raydir.xyz * lastMin;

                    // confirm that hit 
                    if (processing.gBarycentric.w <= 9999.f && material.diffuseColor.w > 0.001f) { opaque = true; };
                };
            } else { fullLength = maxT; };
        };
        rayQueryTerminateEXT(rayQuery);

        // 
        if (fullLength <= (maxT-1.f) && !opaque) { restart = true; };
        if (!restart) { break; };
    };

    // 
    if (fullLength <= (maxT-1.f)) { confirmed = processing; };
    return confirmed;
};


STATIC const float4 sphere = float4(float3(16.f,128.f,16.f), 8.f);

void directLight(in float3 normal, in float3 gnormal, in float4 sphere, in float3 origin, inout uint2 seed, inout float4 gSignal, inout float4 gEnergy) {
    const float3 lightp = sphere.xyz + randomSphere(seed) * sphere.w; float shdist = distance(lightp.xyz,origin.xyz);
    const float3 lightd = normalize(lightp.xyz - origin.xyz);
    const float3 lightc = 128.f*512.f.xxx/(sphere.w*sphere.w);

    float4 signal = float4(0.f.xxx, 0.f);
    if ( dot(gnormal, lightd) >= 0.f ) {
        float sdepth = raySphereIntersect(origin.xyz,lightd,sphere.xyz,sphere.w); //sdepth = min(sdepth, shdist);
        XHIT result = traceRays(origin + faceforward(gnormal, lightd, -gnormal) * 0.0001f, lightd, sdepth, true, 0.01f); // Fixed shadows here (after normals shifting)
        if ( result.gBarycentric.w >= sdepth && sdepth <= 9999.f ) { // If intersects with light
            const float cos_a_max = sqrt(1.f - clamp(sphere.w * sphere.w / dot(sphere.xyz-origin.xyz, sphere.xyz-origin.xyz), 0.f, 1.f));
            signal = float4(gEnergy.xyz * 2.f * (1.f - cos_a_max) * clamp(dot( lightd, normal ), 0.f, 1.f) * lightc, 1.f);
        };
    };
    gSignal += signal, gEnergy *= 0.f.xxxx;
};

void directLight(in float3 normal, in float3 gnormal, in float3 origin, inout uint2 seed, inout float4 gSignal, inout float4 gEnergy) {
    directLight(normal, gnormal, sphere, origin, seed, gSignal, gEnergy);
};

#endif
