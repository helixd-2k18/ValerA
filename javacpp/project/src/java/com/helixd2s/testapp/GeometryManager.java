package com.helixd2s.testapp;

import com.helixd2s.valera.VKt;
import com.helixd2s.valera.ValerABase;
import org.lwjgl.vulkan.VkVertexInputAttributeDescription;
import org.lwjgl.vulkan.VkVertexInputBindingDescription;

public class GeometryManager {
    public VRenderer vrenderer;
    public VkVertexInputBindingDescription binding;
    public VkVertexInputAttributeDescription vattrib;
    public VkVertexInputAttributeDescription nattrib;
    public VkVertexInputAttributeDescription tattrib;

    // can be edit!
    public ValerABase.Geometry geometry;

    // needs to fill!
    public ValerABase.SetBase vertexData;
    public ValerABase.SetBase indexData;

    //
    GeometryManager(VRenderer renderer, int gidx) {
        geometry = vrenderer.geometries[gidx];
        vertexData = vrenderer.vertexData[gidx];
        indexData = vrenderer.indexData[gidx];
        binding = VkVertexInputBindingDescription.create(vrenderer.bindingSet.get(gidx));
        vattrib = VkVertexInputAttributeDescription.create(vrenderer.attributeSet.get(gidx*3+0));
        nattrib = VkVertexInputAttributeDescription.create(vrenderer.attributeSet.get(gidx*3+1));
        tattrib = VkVertexInputAttributeDescription.create(vrenderer.attributeSet.get(gidx*3+2));

        // vertex buffer binding
        int vertexBufferID = gidx*2+0, indexBufferID = gidx*2+1;
        binding.binding(vertexBufferID);
        vattrib.binding(gidx);
        nattrib.binding(gidx);
        tattrib.binding(gidx);

        // should to be match
        var desc = geometry.getDesc();
        desc.vertexAttribute().put(gidx*3+0);
        desc.attributes().put(0, gidx*3+1);
        desc.attributes().put(1, gidx*3+2);

        // matching buffers
        vrenderer.bufferViewSet.get(vertexBufferID).put(vrenderer.vertexData[gidx]);
        if (desc.indexType().get() != 1000165000) {
            desc.indexBufferView().put(indexBufferID);
            vrenderer.bufferViewSet.get(indexBufferID).put(vrenderer.indexData[gidx]);
        }

    }

}
