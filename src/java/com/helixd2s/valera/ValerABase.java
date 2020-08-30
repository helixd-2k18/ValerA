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
    };


    //
    @Name("vlj::AttributeSet")
    public static class AttributeSet extends SetBase {};

    //
    @Name("vlj::BindingSet")
    public static class BindingSet extends SetBase {};

    //
    @Name("vlj::MaterialSet")
    public static class MaterialSet extends SetBase {};

    //
    @Name("vlj::Constants")
    public static class Constants extends SetBase {};

    //
    //@Name("vlj::GeometrySet")
    //public static class GeometrySet extends SetBase {};

    //
    @Name("vlj::InstanceSet")
    public static class InstanceSet extends SetBase {};


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
    };


};
