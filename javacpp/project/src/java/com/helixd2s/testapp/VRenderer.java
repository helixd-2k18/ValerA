package com.helixd2s.testapp;

//
import com.helixd2s.valera.VKt;
import com.helixd2s.valera.ValerABase;
import com.helixd2s.valera.ValerACore;
import org.bytedeco.javacpp.LongPointer;
import org.lwjgl.vulkan.*;

import static org.lwjgl.vulkan.VK10.*;

//
public class VRenderer {

    //
    public int width = 1600, height = 1200;

    //
    public VkInstance vInstance;
    public VkPhysicalDevice vPhysicalDevice;
    public VkDevice vDevice;
    public ValerACore.Driver vDriver;
    public LongPointer vInstanceHandle;
    public LongPointer vDeviceHandle;
    public LongPointer vPhysicalDeviceHandle;

    //
    public ValerABase.Framebuffer framebuffer;
    public ValerABase.PipelineLayout pipelineLayout;
    public ValerABase.TextureSet textureSet;
    public ValerABase.SamplerSet samplerSet;
    public ValerABase.MaterialSet materialSet;
    public ValerABase.Background background;

    public ValerABase.BindingSet bindingSet;
    public ValerABase.AttributeSet attributeSet;
    public ValerABase.BufferViewSet bufferViewSet;
    public ValerABase.VertexSet vertexSet;

    //
    public int geometrySetCount = 0;
    public ValerABase.GeometrySet geometrySets[];
    public ValerABase.Acceleration accelerations[];

    //
    public ValerABase.Acceleration accelerationInstanced;
    public ValerABase.InstanceSet instanceSet;

    //
    public ValerABase.Geometry geometries[];

    //
    public ValerABase.SetBase vertexData[];
    public ValerABase.SetBase indexData[];

    //
    public int materialList[];
    public int geometryList[];
    public int textureList[];

    //
    public int materialCount = 0;
    public int geometryCount = 0;
    public int textureCount = 0;

    //
    public int maxMaterialCount = 256;
    public int maxGeometryCount = 256;
    public int maxTextureCount = 256;

    //
    public int VERTEX_STRIDE = 48; // 4x vertex, 4x normal, 4x texcoord

    //
    void removeGeometry(int idx) {
        if (idx >= 0) {
            int lidx = --geometryCount;
            geometryList[lidx] = idx;
        }
    }

    //
    void removeTexture(int idx) {
        if (idx >= 0) {
            int lidx = --textureCount;
            textureList[lidx] = idx;
        }
    }

    //
    void removeMaterial(int idx) {
        if (idx >= 0) {
            int lidx = --materialCount;
            materialList[lidx] = idx;
        }
    }

    //
    GeometryManager manageGeometry(int gidx) {
        return (new GeometryManager(this, gidx));
    }

    //
    //int createGeometrySet(ValerABase.Geometry geometries[]) {
    int createGeometrySet(int gim[]) {
        ValerACore.DataSetCreateInfo info = new ValerACore.DataSetCreateInfo();
        info.count().put(gim.length);

        //
        var geometrySet = new ValerABase.GeometrySet(vertexSet.uniPtr(), info);
        for (int i=0;i<gim.length;i++) {
            geometrySet.pushGeometry(geometries[gim[i]].uniPtr());
        }

        //
        int gdx = geometrySetCount++;
        geometrySets[gdx] = geometrySet;
        return gdx;
    }

    // but for you needs manager
    int addGeometry(ValerACore.GeometryDesc desc, int vertexCount, int indexCount) {
        if (geometryCount < maxGeometryCount) {
            int lidx = geometryCount++, gidx = geometryList[lidx]; geometryList[lidx] = -1;
            geometries[gidx] = new ValerABase.Geometry(vertexSet.uniPtr(), desc);

            //
            ValerACore.DataSetCreateInfo info = new ValerACore.DataSetCreateInfo();
            info.count().put(VERTEX_STRIDE * vertexCount);
            bufferViewSet.get(gidx*2+0).put(vertexData[gidx] = new ValerABase.SetBase(vDriver.uniPtr(), info));

            //
            if (desc.indexType().get() != 1000165000 && indexCount > 0) {
                int INDEX_STRIDE = 4; // TODO: index format stride
                int indexID = gidx * 2 + 1;
                desc.indexBufferView().put(indexID);
                info.count().put(INDEX_STRIDE * indexCount);
                bufferViewSet.get(indexID).put(indexData[gidx] = new ValerABase.SetBase(vDriver.uniPtr(), info));
            }

            return gidx;
        }
        return -1;
    }

    //
    int addMaterial(ValerACore.MaterialUnit unit) {
        if (materialCount < maxMaterialCount) {
            int lidx = materialCount++, midx = materialList[lidx]; materialList[lidx] = -1;
            materialSet.get(midx).put(unit);
            return midx;
        }
        return -1;
    }

    //
    int addTexture(VKt.ImageRegion image) {
        if (textureCount < maxTextureCount) {
            int lidx = textureCount++, tidx = textureList[lidx]; textureList[lidx] = -1;
            textureSet.get(tidx).put(image);
            return tidx;
        }
        return -1;
    }

    //
    void init(int width, int height) {
        //
        vDriver = new ValerACore.Driver();
        vInstanceHandle = vDriver.createInstance();
        vInstance = new VkInstance(vInstanceHandle.get(), VkInstanceCreateInfo.create(vDriver.getInstanceCreateInfoAddress()));
        vPhysicalDeviceHandle = vDriver.getPhysicalDevice();
        vPhysicalDevice = new VkPhysicalDevice(vPhysicalDeviceHandle.get(), vInstance);
        vDeviceHandle = vDriver.createDevice();
        vDevice = new VkDevice(vDeviceHandle.get(), vPhysicalDevice, VkDeviceCreateInfo.create(vDriver.getDeviceCreateInfoAddress()));

        //
        ValerACore.DataSetCreateInfo info = new ValerACore.DataSetCreateInfo();

        //
        framebuffer = new ValerABase.Framebuffer(vDriver.uniPtr());
        pipelineLayout = new ValerABase.PipelineLayout(vDriver.uniPtr());
        textureSet = new ValerABase.TextureSet(vDriver.uniPtr());
        samplerSet = new ValerABase.SamplerSet(vDriver.uniPtr());
        background = new ValerABase.Background(vDriver.uniPtr());

        //
        info.count().put(maxMaterialCount);
        materialSet = new ValerABase.MaterialSet(vDriver.uniPtr(), info);
        textureSet = new ValerABase.TextureSet(vDriver.uniPtr());

        //
        info.count().put(maxGeometryCount);
        bindingSet = new ValerABase.BindingSet(vDriver.uniPtr(), info);

        //
        info.count().put(maxGeometryCount * 3);
        attributeSet = new ValerABase.AttributeSet(vDriver.uniPtr(), info);

        //
        bufferViewSet = new ValerABase.BufferViewSet(vDriver.uniPtr());

        //
        var vis = new ValerACore.VertexSetCreateInfo();
        vis.bindings().put(bindingSet);
        vis.attributes().put(attributeSet);
        vis.bufferViews().put(bufferViewSet);
        vertexSet = new ValerABase.VertexSet(vDriver.uniPtr(), vis);

        //
        framebuffer.createFramebuffer(this.width = width, this.height = height);

        //
        geometryList = new int[maxGeometryCount];
        materialList = new int[maxMaterialCount];
        textureList = new int[maxTextureCount];
        vertexData = new ValerABase.SetBase[maxGeometryCount];
        indexData = new ValerABase.SetBase[maxGeometryCount];

        //
        for (int i=0;i<maxMaterialCount;i++) { materialList[i] = i; };
        for (int i=0;i<maxGeometryCount;i++) { geometryList[i] = i; };
        for (int i=0;i<maxTextureCount;i++) { textureList[i] = i; };


        // Test Sampler
        var smpc = VkSamplerCreateInfo.calloc();
        smpc.sType(VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO);
        smpc.pNext(0);
        smpc.magFilter(VK_FILTER_LINEAR);
        smpc.minFilter(VK_FILTER_LINEAR);
        smpc.addressModeU(VK_SAMPLER_ADDRESS_MODE_REPEAT);
        smpc.addressModeV(VK_SAMPLER_ADDRESS_MODE_REPEAT);


    }

}
