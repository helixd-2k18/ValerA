package com.helixd2s.testapp;

//
import com.helixd2s.valera.VKt;
import com.helixd2s.valera.ValerABase;
import com.helixd2s.valera.ValerACore;
import org.bytedeco.javacpp.LongPointer;
import org.lwjgl.vulkan.*;

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
    public ValerABase.Background background;
    public ValerABase.MaterialSet materialSet;

    public ValerABase.BindingSet bindingSet;
    public ValerABase.AttributeSet attributeSet;
    public ValerABase.BufferViewSet bufferViewSet;
    public ValerABase.InstanceSet instanceSet;
    public ValerABase.VertexSet vertexSet;

    //
    public ValerABase.GeometrySet geometries[];
    public ValerABase.Acceleration accelerations[];

    //
    public ValerABase.Acceleration accelerationInstanced;

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
    int addGeometry(ValerACore.GeometryDesc desc) {
        if (geometryCount < maxGeometryCount) {
            int lidx = geometryCount++;
            int gidx = geometryList[lidx];
            geometryList[lidx] = -1;

            var bufferView = bufferViewSet.get(lidx);
            var binding = VkVertexInputBindingDescription.create(bindingSet.get(gidx));
            var vattrib = VkVertexInputAttributeDescription.create(attributeSet.get(gidx*3+0));
            var nattrib = VkVertexInputAttributeDescription.create(attributeSet.get(gidx*3+1));
            var tattrib = VkVertexInputAttributeDescription.create(attributeSet.get(gidx*3+2));

            // TODO:

            return gidx;
        }
        return -1;
    }

    //
    int addMaterial(ValerACore.MaterialUnit unit) {
        if (materialCount < maxMaterialCount) {
            int lidx = materialCount++;
            int midx = materialList[lidx];
            materialList[lidx] = -1;

            materialSet.get(midx).put(unit);

            return midx;
        }
        return -1;
    }

    //
    int addTexture(VKt.ImageRegion image) {
        if (textureCount < maxTextureCount) {
            int lidx = textureCount++;
            int tidx = textureList[lidx];
            textureList[lidx] = -1;

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
        framebuffer.createFramebuffer(this.width = width, this.height = height);

        //
        geometryList = new int[maxGeometryCount];
        materialList = new int[maxMaterialCount];
        textureList = new int[maxTextureCount];

        //
        for (int i=0;i<maxMaterialCount;i++) { materialList[i] = i; };
        for (int i=0;i<maxGeometryCount;i++) { geometryList[i] = i; };
        for (int i=0;i<maxTextureCount;i++) { textureList[i] = i; };

    }

}
