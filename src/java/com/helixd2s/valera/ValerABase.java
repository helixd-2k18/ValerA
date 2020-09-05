package com.helixd2s.valera;

import org.bytedeco.javacpp.*;
import org.bytedeco.javacpp.annotation.*;
import org.bytedeco.javacpp.tools.*;
import org.bytedeco.javacpp.indexer.*;
import java.lang.annotation.*;

// "jniJiviXBase", "JiviX"

// TODO: Wrap CORE Instead Of External Native... (i.e. reduce layers count)
@Platform(include = {
        "./include/vkt3/fw.hpp",
        "./include/stdafx.h",
        "jniVKt.h",
        "jniValerACore.h",
}, link = {"vulkan-1", "glfw"}, define = {"ENABLE_OPENGL_INTEROP", "VKT_USE_GLAD", "WIN32", "OS_WIN", "VK_ENABLE_BETA_EXTENSIONS", "VK_USE_PLATFORM_WIN32_KHR", "SHARED_PTR_NAMESPACE std", "UNIQUE_PTR_NAMESPACE std", "VKT_ENABLE_GLFW_SUPPORT"})
@Name("") //
public class ValerABase implements InfoMapper {
    static { Loader.load(); };
    
    @Documented
    @Retention(RetentionPolicy.RUNTIME)
    @Target({ElementType.METHOD, ElementType.PARAMETER})
    @Adapter("vkt::uni_ptr")
    public @interface UniPtr {
        /** template type */
        String value() default "";
    };

    // 
    public void map(InfoMap infoMap) {
        infoMap.put(new Info("vkt::uni_ptr").skip().annotations("@UniPtr"));
    };



    // 
    @Name("vlj::SetBase")
    public static class SetBase extends Pointer {
        static { Loader.load(); }

        // 
        public SetBase(@UniPtr ValerACore.SetBase object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.SetBase object);

        // 
        public SetBase(Pointer p) { super(p); };
        public SetBase(@UniPtr VKt.Driver driver, ValerACore.DataSetCreateInfo info) { allocate(driver, info); };
        public SetBase(@UniPtr VKt.Driver driver) { allocate(driver); };
        public SetBase() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
        private native void allocate(@UniPtr VKt.Driver driver, ValerACore.DataSetCreateInfo info);

        // 
        public native @UniPtr ValerACore.SetBase uniPtr();
        public native @SharedPtr ValerACore.SetBase sharedPtr();
        public native void createDescriptorSet(@ByRef ValerACore.PipelineLayout pipelineLayout);

        // 
        public native @ByRef @UniPtr VKt.Vector getCpuBuffer();
        public native @ByRef @UniPtr VKt.Vector getGpuBuffer();
    };


    //
    @Name("vlj::AttributeSet")
    public static class AttributeSet extends SetBase {
        // 
        public AttributeSet(@UniPtr ValerACore.AttributeSet object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.AttributeSet object);

        // 
        public native @UniPtr ValerACore.AttributeSet uniPtr();
        public native @SharedPtr ValerACore.AttributeSet sharedPtr();

        // 
        public native @Cast("long") @ByRef long get(int I);
        public native @Cast("long") @ByRef long get();
        public native void createDescriptorSet(@ByRef ValerACore.PipelineLayout pipelineLayout);
    };

    //
    @Name("vlj::BindingSet")
    public static class BindingSet extends SetBase {
        // 
        public BindingSet(@UniPtr ValerACore.BindingSet object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.BindingSet object);

        // 
        public native @UniPtr ValerACore.BindingSet uniPtr();
        public native @SharedPtr ValerACore.BindingSet sharedPtr();

        // 
        public native @Cast("long") @ByRef long get(int I);
        public native @Cast("long") @ByRef long get();
        public native void createDescriptorSet(@ByRef ValerACore.PipelineLayout pipelineLayout);
    };

    //
    @Name("vlj::MaterialSet")
    public static class MaterialSet extends SetBase {
        // 
        public MaterialSet(@UniPtr ValerACore.MaterialSet object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.MaterialSet object);

        // 
        public native @UniPtr ValerACore.MaterialSet uniPtr();
        public native @SharedPtr ValerACore.MaterialSet sharedPtr();

        // 
        public native ValerACore.MaterialUnit get(int I);
        public native void createDescriptorSet(@ByRef ValerACore.PipelineLayout pipelineLayout);
    };

    //
    @Name("vlj::Constants")
    public static class Constants extends SetBase {
        // 
        public Constants(@UniPtr ValerACore.Constants object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.Constants object);

        // 
        public native @UniPtr ValerACore.Constants uniPtr();
        public native @SharedPtr ValerACore.Constants sharedPtr();

        // 
        public native ValerACore.ConstantDesc get(int I);
        public native ValerACore.ConstantDesc get();
        public native void createDescriptorSet(@ByRef ValerACore.PipelineLayout pipelineLayout);
    };

    //
    //@Name("vlj::GeometrySet")
    //public static class GeometrySet extends SetBase {};

    //
    @Name("vlj::InstanceSet")
    public static class InstanceSet extends SetBase {
        // 
        public InstanceSet(@UniPtr ValerACore.InstanceSet object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.InstanceSet object);

        // 
        public native @UniPtr ValerACore.InstanceSet uniPtr();
        public native @SharedPtr ValerACore.InstanceSet sharedPtr();

        // 
        public native ValerACore.VsGeometryInstance get(int I);
        public native void createDescriptorSet(@ByRef ValerACore.PipelineLayout pipelineLayout);
    };


    // 
    @Name("vlj::VertexSet")
    public static class VertexSet extends Pointer {
        static { Loader.load(); }

        // 
        public VertexSet(@UniPtr ValerACore.VertexSet object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.VertexSet object);
        
        // 
        public VertexSet(Pointer p) { super(p); };
        public VertexSet(@UniPtr VKt.Driver driver, ValerACore.VertexSetCreateInfo info) { allocate(driver, info); };
        public VertexSet(@UniPtr VKt.Driver driver) { allocate(driver); };
        public VertexSet() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
        private native void allocate(@UniPtr VKt.Driver driver, ValerACore.VertexSetCreateInfo info);

        // 
        public native @UniPtr ValerACore.VertexSet uniPtr();
        public native @SharedPtr ValerACore.VertexSet sharedPtr();

        //
        public native void createDescriptorSet(@ByRef ValerACore.PipelineLayout pipelineLayout);
    };

    //
    @Name("vlj::Acceleration")
    public static class Acceleration extends Pointer {
        static { Loader.load(); }

        // 
        public Acceleration(@UniPtr ValerACore.Acceleration object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.Acceleration object);

        // 
        public Acceleration(Pointer p) { super(p); };
        public Acceleration(@UniPtr VKt.Driver driver, ValerACore.AccelerationCreateInfo info) { allocate(driver, info); };
        public Acceleration(@UniPtr VKt.Driver driver) { allocate(driver); };
        public Acceleration() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver, ValerACore.AccelerationCreateInfo info);
        private native void allocate(@UniPtr VKt.Driver driver);

        // 
        public native @UniPtr ValerACore.Acceleration uniPtr();
        public native @SharedPtr ValerACore.Acceleration sharedPtr();

        // 
        public native void createDescriptorSet(@ByRef ValerACore.PipelineLayout pipelineLayout);

        // 
        public native void updateAccelerationStructure(ValerACore.AccelerationCreateInfo info);
        public native void setCommand(@Cast("VkCommandBuffer") long info);
        public native @Cast("int64_t") long getHandle();
    };

    //
    @Name("vlj::Geometry")
    public static class Geometry extends Pointer {
        static { Loader.load(); }

        // 
        public Geometry(@UniPtr ValerACore.Geometry object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.Geometry object);
        
        // 
        public Geometry(Pointer p) { super(p); };
        public Geometry(@UniPtr ValerACore.VertexSet vset, ValerACore.GeometryDesc info) { allocate(vset, info); };
        public Geometry(@UniPtr ValerACore.VertexSet vset) { allocate(vset); };
        public Geometry() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr ValerACore.VertexSet vset, ValerACore.GeometryDesc info);
        private native void allocate(@UniPtr ValerACore.VertexSet vset);

        // 
        public native @UniPtr ValerACore.Geometry uniPtr();
        public native @SharedPtr ValerACore.Geometry sharedPtr();

        //
        public native void setIndexBuffer(@Cast("uint32_t") int indexBufferView, @Cast("VkIndexType") int indexType);
        public native void setVertexBuffer(@Cast("uint32_t") int vertexAttribute);
    };

    // 
    @Name("vlj::GeometrySet")
    public static class GeometrySet extends Pointer {
        static { Loader.load(); }

        // 
        public GeometrySet(@UniPtr ValerACore.GeometrySet object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.GeometrySet object);

        // 
        public GeometrySet(Pointer p) { super(p); };
        public GeometrySet() { allocate(); };
        //public GeometrySet(@UniPtr VKt.Driver driver, DataSetCreateInfo info) { allocate(driver, info); };
        //public GeometrySet(@UniPtr VKt.Driver driver) { allocate(driver); };
        public GeometrySet(@UniPtr ValerACore.VertexSet driver, ValerACore.DataSetCreateInfo info) { allocate(driver, info); };
        public GeometrySet(@UniPtr ValerACore.VertexSet driver) { allocate(driver); };

        // 
        private native void allocate();
        //private native void allocate(@UniPtr VKt.Driver driver);
        //private native void allocate(@UniPtr VKt.Driver driver, DataSetCreateInfo info);
        private native void allocate(ValerACore.VertexSet driver);
        private native void allocate(ValerACore.VertexSet driver, ValerACore.DataSetCreateInfo info);

        // 
        public native @UniPtr ValerACore.GeometrySet uniPtr();
        public native @SharedPtr ValerACore.GeometrySet sharedPtr();

        // 
        public native void createDescriptorSet(@ByRef ValerACore.PipelineLayout pipelineLayout);
        public native ValerACore.GeometryDesc get(int I);
    };


    //
    @Name("vlj::Background")
    public static class Background extends Pointer {
        static { Loader.load(); }

        // 
        public Background(@UniPtr ValerACore.Background object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.Background object);

        // 
        public Background(Pointer p) { super(p); };
        public Background(@UniPtr VKt.Driver driver) { allocate(driver); };
        public Background(@UniPtr VKt.Driver driver, VKt.ImageRegion region) { allocate(driver, region); };
        public Background() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
        private native void allocate(@UniPtr VKt.Driver driver, VKt.ImageRegion region);

        // 
        public native @UniPtr ValerACore.Background uniPtr();
        public native @SharedPtr ValerACore.Background sharedPtr();

        // 
        public native void createDescriptorSet(@ByRef ValerACore.PipelineLayout pipelineLayout);
        public native void setImage(@ByRef VKt.ImageRegion image);
    };

    //
    @Name("vlj::BufferViewSet")
    public static class BufferViewSet extends Pointer {
        static { Loader.load(); }

        // 
        public BufferViewSet(@UniPtr ValerACore.BufferViewSet object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.BufferViewSet object);

        // 
        public BufferViewSet(Pointer p) { super(p); };
        public BufferViewSet(@UniPtr VKt.Driver driver) { allocate(driver); };
        public BufferViewSet() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);

        // 
        public native @UniPtr ValerACore.BufferViewSet uniPtr();
        public native @SharedPtr ValerACore.BufferViewSet sharedPtr();

        // 
        public native @ByRef VKt.Vector get(int I);

        //
        public native void createDescriptorSet(@ByRef ValerACore.PipelineLayout pipelineLayout);
        public native void pushBufferView(@ByRef VKt.Vector vector);
        public native void resetBufferViews();
    };

    //
    @Name("vlj::TextureSet")
    public static class TextureSet extends Pointer {
        static { Loader.load(); }

        // 
        public TextureSet(@UniPtr ValerACore.TextureSet object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.TextureSet object);

        // 
        public TextureSet(Pointer p) { super(p); };
        public TextureSet(@UniPtr VKt.Driver driver) { allocate(driver); };
        public TextureSet() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);

        // 
        public native @UniPtr ValerACore.TextureSet uniPtr();
        public native @SharedPtr ValerACore.TextureSet sharedPtr();

        // 
        public native @ByRef VKt.ImageRegion get(int I);

        //
        public native void createDescriptorSet(@ByRef ValerACore.PipelineLayout pipelineLayout);
        public native void pushImage(@ByRef VKt.ImageRegion image);
        public native void resetImages();
    };

    //
    @Name("vlj::SamplerSet")
    public static class SamplerSet extends Pointer {
        static { Loader.load(); }

        // 
        public SamplerSet(@UniPtr ValerACore.SamplerSet object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.SamplerSet object);

        // 
        public SamplerSet(Pointer p) { super(p); };
        public SamplerSet(@UniPtr VKt.Driver driver) { allocate(driver); };
        public SamplerSet() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);

        // 
        public native @UniPtr ValerACore.SamplerSet uniPtr();
        public native @SharedPtr ValerACore.SamplerSet sharedPtr();

        // 
        public native @Cast("long*") @ByRef LongPointer get(int I);

        //
        public native void createDescriptorSet(@ByRef ValerACore.PipelineLayout pipelineLayout);
        public native void pushSampler(@Cast("VkSampler") @ByRef long sampler);
        public native void resetSamplers();
    };
    



    // 
    @Name("vlj::RenderCommand")
    public static class RenderCommand extends Pointer {
        static { Loader.load(); }

        // 
        public RenderCommand(@UniPtr ValerACore.RenderCommand object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.RenderCommand object);

        // 
        public RenderCommand(Pointer p) { super(p); };
        public RenderCommand(@UniPtr VKt.Driver driver, ValerACore.RenderCommandCreateInfo info) { allocate(driver, info); };
        public RenderCommand(@UniPtr VKt.Driver driver) { allocate(driver); };
        public RenderCommand() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
        private native void allocate(@UniPtr VKt.Driver driver, ValerACore.RenderCommandCreateInfo info);

        // 
        public native @UniPtr ValerACore.RenderCommand uniPtr();
        public native @SharedPtr ValerACore.RenderCommand sharedPtr();
        
        //
        public native void setRasterization(@UniPtr ValerACore.Rasterization rasterization);
        public native void setRayTracing(@UniPtr ValerACore.RayTracing rayTracing);
        public native void setCommand(@ByRef @Cast("VkCommandBuffer") long commandBuffer);
        public native void setDescriptorSets(@ByRef ValerACore.PipelineLayout pipelineLayout);
    };

    // 
    @Name("vlj::BuildCommand")
    public static class BuildCommand extends Pointer {
        static { Loader.load(); }

        // 
        public BuildCommand(@UniPtr ValerACore.BuildCommand object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.BuildCommand object);

        // 
        public BuildCommand(Pointer p) { super(p); };
        public BuildCommand(@UniPtr VKt.Driver driver, ValerACore.BuildCommandCreateInfo info) { allocate(driver, info); };
        public BuildCommand(@UniPtr VKt.Driver driver) { allocate(driver); };
        public BuildCommand() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
        private native void allocate(@UniPtr VKt.Driver driver, ValerACore.BuildCommandCreateInfo info);

        // 
        public native @UniPtr ValerACore.BuildCommand uniPtr();
        public native @SharedPtr ValerACore.BuildCommand sharedPtr();

        //
        public native void setAccelerationTop(@UniPtr @ByRef ValerACore.Acceleration acceleration);
        public native void setDescriptorSets(@UniPtr @ByRef ValerACore.PipelineLayout pipelineLayout);
        public native void setCommand(@ByRef @Cast("VkCommandBuffer") long commandBuffer);
        public native void setCommandTop(@ByRef @Cast("VkCommandBuffer") long commandBuffer);
    };

    // 
    @Name("vlj::RayTracing")
    public static class RayTracing extends Pointer {
        static { Loader.load(); }

        // 
        public RayTracing(@UniPtr ValerACore.RayTracing object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.RayTracing object);

        // 
        public RayTracing(Pointer p) { super(p); };
        public RayTracing(@UniPtr VKt.Driver driver, ValerACore.RayTracingCreateInfo info) { allocate(driver, info); };
        public RayTracing(@UniPtr VKt.Driver driver) { allocate(driver); };
        public RayTracing() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
        private native void allocate(@UniPtr VKt.Driver driver, ValerACore.RayTracingCreateInfo info);

        // 
        public native @UniPtr ValerACore.RayTracing uniPtr();
        public native @SharedPtr ValerACore.RayTracing sharedPtr();

        // 
        public native void setCommand(@ByRef @Cast("VkCommandBuffer") long commandBuffer);
        public native void setCommandFinal(@ByRef @Cast("VkCommandBuffer") long commandBuffer);

        // 
        public native void setDescriptorSets(@UniPtr @ByRef ValerACore.PipelineLayout pipelineLayout);
        public native void setAccelerationTop(@UniPtr @ByRef ValerACore.Acceleration acceleration);
        public native void setConstants(@UniPtr @ByRef ValerACore.Constants constants);

        //
        //public native @UniPtr VKt.UIntVector getCounters();
        //public native @UniPtr ValerACore.Constants getConstants();
    };

    // 
    @Name("vlj::Rasterization")
    public static class Rasterization extends Pointer {
        static { Loader.load(); }

        // 
        public Rasterization(@UniPtr ValerACore.Rasterization object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.Rasterization object);

        // 
        public Rasterization(Pointer p) { super(p); };
        public Rasterization(@UniPtr VKt.Driver driver, ValerACore.PipelineCreateInfo info) { allocate(driver, info); };
        public Rasterization(@UniPtr VKt.Driver driver) { allocate(driver); };
        public Rasterization() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
        private native void allocate(@UniPtr VKt.Driver driver, ValerACore.PipelineCreateInfo info);

        // 
        public native @UniPtr ValerACore.Rasterization uniPtr();
        public native @SharedPtr ValerACore.Rasterization sharedPtr();

        // 
        public native void drawCommand(@ByRef @Cast("VkCommandBuffer") long commandBuffer);
        public native void setCommand(@ByRef @Cast("VkCommandBuffer") long commandBuffer);
        public native void setDescriptorSets(@UniPtr @ByRef ValerACore.PipelineLayout pipelineLayout);
        public native void setInstanceSet(@UniPtr @ByRef ValerACore.InstanceSet acceleration);
        public native void setConstants(@UniPtr @ByRef ValerACore.Constants constants);

        //
        //public native @UniPtr ValerACore.Constants getConstants();
    };

    // 
    @Name("vlj::PipelineLayout")
    public static class PipelineLayout extends Pointer {
        static { Loader.load(); }

        // 
        public PipelineLayout(@UniPtr ValerACore.PipelineLayout object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.PipelineLayout object);

        // 
        public PipelineLayout(Pointer p) { super(p); };
        public PipelineLayout(@UniPtr VKt.Driver driver) { allocate(driver); };
        public PipelineLayout() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);

        // 
        public native @UniPtr ValerACore.PipelineLayout uniPtr();
        public native @SharedPtr ValerACore.PipelineLayout sharedPtr();

        // 
        public native void setMaterials(@UniPtr ValerACore.MaterialSet materialSet, @UniPtr ValerACore.TextureSet textureSet, @UniPtr ValerACore.SamplerSet samplerSet);
        public native @Name("setVertexSet<vlr::VertexSet>") void setVertexSet(@UniPtr ValerACore.VertexSet vertexData);
        public native @Name("setFramebuffer<vlr::Framebuffer>") void setFramebuffer(@UniPtr ValerACore.Framebuffer framebuffer);
        public native @Name("setAccelerationTop<vlr::Acceleration>") void setAccelerationTop(@UniPtr ValerACore.Acceleration acceleration);
        public native @Name("setConstants<vlr::Constants>") void setConstants(@UniPtr ValerACore.Constants constants);
        public native @Name("setRayTracing<vlr::RayTracing>") void setRayTracing(@UniPtr ValerACore.RayTracing rayTracing);
        public native @Name("setRasterization<vlr::Rasterization>") void setRasterization(@UniPtr ValerACore.Rasterization rayTracing);
        public native @Name("setInstanceSet<vlr::InstanceSet>") void setInstanceSet(@UniPtr ValerACore.InstanceSet instanceSet);
        public native @Name("setBackground<vlr::Background>") void setBackground(@UniPtr ValerACore.Background background);

        // 
        public native @StdVector LongPointer getDescriptorSets();

        // 
        public native @Cast("int64_t") @ByRef long getDescriptorSet(int I);
        public native @Cast("int64_t") @ByRef long getDescriptorSetLayout(int I);
        public native @Cast("int64_t") @ByRef long getPipelineCache();
        public native @Cast("int64_t") @ByRef long getBindingPipelineLayout();
        public native @Cast("int64_t") @ByRef long getTransformPipelineLayout();
        public native @Cast("int64_t") @ByRef long getDescriptorPool();
        public native @Cast("int64_t") @ByRef long getPipelineLayout();
    };

    // 
    @Name("vlj::Framebuffer")
    public static class Framebuffer extends Pointer {
        static { Loader.load(); }

        // 
        public Framebuffer(@UniPtr ValerACore.Framebuffer object) { allocate(object); };
        private native void allocate(@UniPtr ValerACore.Framebuffer object);

        // 
        public Framebuffer(Pointer p) { super(p); };
        public Framebuffer(@UniPtr VKt.Driver driver) { allocate(driver); };
        public Framebuffer() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);

        // 
        public native @UniPtr ValerACore.Framebuffer uniPtr();
        public native @SharedPtr ValerACore.Framebuffer sharedPtr();

        //
        public native void createRenderPass();
        public native void createDescriptorSet(@UniPtr @ByRef ValerACore.PipelineLayout pipelineLayout);
        public native void createFramebuffer(@Cast("uint32_t") int width, @Cast("uint32_t") int height);

        //
        public native void imageToLinearCopyCommand(@ByRef @Cast("VkCommandBuffer") long commandBuffer, @ByRef @Cast("uint32_t") int whatDenoise);
        public native void linearToImageCopyCommand(@ByRef @Cast("VkCommandBuffer") long commandBuffer, @ByRef @Cast("uint32_t") int whatDenoise);
    };


};
