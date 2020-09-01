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
}, link = {"vulkan-1", "glfw"}, define = {"ENABLE_OPENGL_INTEROP", "VKT_USE_GLAD", "WIN32", "OS_WIN", "VK_ENABLE_BETA_EXTENSIONS", "VK_USE_PLATFORM_WIN32_KHR", "SHARED_PTR_NAMESPACE std", "UNIQUE_PTR_NAMESPACE std", "VKT_ENABLE_GLFW_SUPPORT"})
@Name("") //
public class ValerACore implements InfoMapper {
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
    @Name("vlj::Driver")
    public static class Driver extends Pointer {
        static { Loader.load(); }

        public Driver(Pointer p) {
            super(p);
        }
        public Driver() {
            allocate();
        }
        public Driver(@UniPtr VKt.Driver object) {
            allocate(object);
        }

        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver object);

        // LWJGL-3 Compatible
        public native void loadXVK();
        public native long getInstanceCreateInfoAddress();
        public native long getDeviceCreateInfoAddress();

        // Get Handle Value
        public native @ByRef @Cast("uintptr_t*") LongPointer getSurface();
        public native @ByRef @Cast("uintptr_t*") LongPointer getSwapchain();
        public native @ByRef @Cast("uintptr_t*") LongPointer getPhysicalDevice();
        public native @ByRef @Cast("uintptr_t*") LongPointer getPhysicalDevice(int ID);
        public native @ByRef @Cast("uintptr_t*") LongPointer getDevice();
        public native @ByRef @Cast("uintptr_t*") LongPointer getQueue();
        public native @ByRef @Cast("uintptr_t*") LongPointer getFence();
        public native @ByRef @Cast("uintptr_t*") LongPointer getInstance();
        public native @ByRef @Cast("uintptr_t*") LongPointer getCommandPool();
        public native @ByRef @Cast("uintptr_t*") LongPointer getPipelineCache();
        public native @ByRef @Cast("uintptr_t*") LongPointer getDescriptorPool();
        public native @ByRef @Cast("uintptr_t*") LongPointer getDepthImageView();
        public native @ByRef @Cast("uintptr_t*") LongPointer getDepthImage();
        public native @ByRef @Cast("uintptr_t*") LongPointer getRenderPass();
        public native @ByRef @Cast("uintptr_t*") LongPointer createInstance();
        public native @ByRef @Cast("uintptr_t*") LongPointer createDevice();
        public native @ByRef @Cast("uintptr_t*") LongPointer createDevice(@Cast("VkPhysicalDevice") long physicalDeviceHandle);

        // Get Address of Reference... (but needs wrapped as Pointer?)
        public native @Name("getMemoryProperties") @ByRef @Cast("int8_t*") BytePointer _getMemoryProperties();
        public native @Name("getAllocator") @ByRef @Cast("int8_t*") BytePointer _getAllocator();
        //public native @Name("getDispatch")          @ByRef @Cast("int8_t*") BytePointer _getDispatch();

        // Get Address from allocator or properties
        //public long getDispatch()         { return this._getDispatch().address(); };
        public long getAllocator() {
            return this._getAllocator().address();
        }
        public long getMemoryProperties() {
            return this._getMemoryProperties().address();
        }

        //
        public native @UniPtr VKt.Device getDeviceDispatch();
        public native @UniPtr VKt.Instance getInstanceDispatch();

        //
        public native long memoryAllocationInfoPtr();
    };
    
    // 
    @Name("vlr::MaterialUnit")
    public static class MaterialUnit extends Pointer {
        static { Loader.load(); }

        public MaterialUnit(Pointer p) { super(p); };
        public MaterialUnit() { allocate(); };

        private native void allocate();

        // Vectors
        public native @MemberGetter @Cast("float*") @ByRef FloatPointer diffuse();
        public native @MemberGetter @Cast("float*") @ByRef FloatPointer pbrAGM();
        public native @MemberGetter @Cast("float*") @ByRef FloatPointer normal();
        public native @MemberGetter @Cast("float*") @ByRef FloatPointer emission();

        // Texture Indices
        public native @MemberGetter @Cast("int*") @ByRef IntPointer diffuseTexture();
        public native @MemberGetter @Cast("int*") @ByRef IntPointer pbrAGMTexture();
        public native @MemberGetter @Cast("int*") @ByRef IntPointer normalTexture();
        public native @MemberGetter @Cast("int*") @ByRef IntPointer emissionTexture();
        
        // 
        public native @MemberGetter @Cast("int*") @ByRef IntPointer udata();
    };

    // 
    @Name("vlr::AccelerationCreateInfo")
    public static class AccelerationCreateInfo extends Pointer {
        static { Loader.load(); }

        public AccelerationCreateInfo(Pointer p) { super(p); };
        public AccelerationCreateInfo() { allocate(); };

        private native void allocate();

        public native @MemberGetter @UniPtr InstanceSet instanceSet();
        public native @MemberGetter @UniPtr GeometrySet geometrySet();
        public native @MemberGetter @StdVector LongPointer initials();

        public native @MemberSetter void instanceSet(@UniPtr InstanceSet instanceSet);
        public native @MemberSetter void geometrySet(@UniPtr GeometrySet geometrySet);
        public native @MemberSetter void initials(@StdVector LongPointer initials);
    };

    // 
    @Name("vlr::DataSetCreateInfo")
    public static class DataSetCreateInfo extends Pointer {
        static { Loader.load(); }

        public DataSetCreateInfo(Pointer p) { super(p); };
        public DataSetCreateInfo() { allocate(); };

        private native void allocate();

        public native @MemberGetter @Cast("long*") LongPointer count();
        public native @MemberGetter @Cast("int8_t*") BytePointer uniform();
        public native @MemberGetter @Cast("int8_t*") BytePointer enableCPU();
        public native @MemberGetter @Cast("int8_t*") BytePointer enableGL();
    };

    // 
    @Name("vlr::VertexSetCreateInfo")
    public static class VertexSetCreateInfo extends Pointer {
        static { Loader.load(); }

        public VertexSetCreateInfo(Pointer p) { super(p); };
        public VertexSetCreateInfo() { allocate(); };

        private native void allocate();

        public native @MemberGetter @UniPtr BindingSet bindings();
        public native @MemberGetter @UniPtr AttributeSet attributes();
        public native @MemberGetter @UniPtr BufferViewSet bufferViews();

        public native @MemberSetter void bindings(@UniPtr BindingSet bindingSet);
        public native @MemberSetter void attributes(@UniPtr AttributeSet attributes);
        public native @MemberSetter void bufferViews(@UniPtr BufferViewSet bufferViews);
    };

    // 
    @Name("vlr::RenderCommandCreateInfo")
    public static class RenderCommandCreateInfo extends Pointer {
        static { Loader.load(); }

        public RenderCommandCreateInfo(Pointer p) { super(p); };
        public RenderCommandCreateInfo() { allocate(); };

        private native void allocate();

        public native @MemberGetter @UniPtr PipelineLayout layout();
        public native @MemberGetter @UniPtr RayTracing rayTracing();
        public native @MemberGetter @UniPtr Rasterization rasterization();

        public native @MemberSetter void layout(@UniPtr PipelineLayout layout);
        public native @MemberSetter void rayTracing(@UniPtr RayTracing rayTracing);
        public native @MemberSetter void rasterization(@UniPtr Rasterization rasterization);
    };

    // 
    @Name("vlr::BuildCommandCreateInfo")
    public static class BuildCommandCreateInfo extends Pointer {
        static { Loader.load(); }

        public BuildCommandCreateInfo(Pointer p) { super(p); };
        public BuildCommandCreateInfo() { allocate(); };

        private native void allocate();

        public native @MemberGetter @UniPtr PipelineLayout layout();
        public native @MemberGetter @UniPtr Acceleration accelerationTop();

        public native @MemberSetter void layout(@UniPtr PipelineLayout layout);
        public native @MemberSetter void accelerationTop(@UniPtr Acceleration acceleration);
    };

    //
    @Name("vkh::VsGeometryInstance")
    public static class VsGeometryInstance extends Pointer {
        static { Loader.load(); }

        public VsGeometryInstance(Pointer p) { super(p); };
        public VsGeometryInstance() { allocate(); };

        private native void allocate();

        // 
        public native @MemberGetter @Cast("float*") @ByRef FloatPointer transform();
        public native @MemberGetter @Cast("byte*") @ByRef @Name("transform") BytePointer first();
        public native @MemberGetter @Cast("long*") @ByRef LongPointer accelerationStructureHandle();

        // 
        public int getInt24(int offset) {
            return (first().get(offset) | (first().get(offset+1)<<8) | (first().get(offset+2)<<16));
        };

        // 
        public void setInt24(int offset, int num24) {
            BytePointer ptr = first();
            ptr.put((long)(offset+0), (byte)(num24&0xFF));
            ptr.put((long)(offset+1), (byte)((num24>>8)&0xFF));
            ptr.put((long)(offset+2), (byte)((num24>>16)&0xFF));
        };

        // By Offsets
        public static int CUSTOM_ID = 48;
        public static int MASK = 51;
        public static int INSTANCE_OFFSET = 52;
        public static int FLAGS = 55;
    };

    //
    @Name("vlr::GeometryDesc")
    public static class GeometryDesc extends Pointer {
        static { Loader.load(); }

        public GeometryDesc(Pointer p) { super(p); };
        public GeometryDesc() { allocate(); };

        private native void allocate();

        public native @MemberGetter @Cast("float*") @ByRef FloatPointer transform();
        public native @MemberGetter @Cast("int*") @ByRef IntPointer firstVertex();
        public native @MemberGetter @Cast("int*") @ByRef IntPointer primitiveCount();
        public native @MemberGetter @Cast("int*") @ByRef IntPointer material();
        public native @MemberGetter @Cast("int8_t*") @ByRef BytePointer mesh_flags();
        public native @MemberGetter @Cast("int*") @ByRef IntPointer vertexAttribute();
        public native @MemberGetter @Cast("int*") @ByRef IntPointer indexBufferView();
        public native @MemberGetter @Cast("int*") @ByRef IntPointer indexType();
        public native @MemberGetter @Cast("int*") @ByRef IntPointer reserved();
        public native @MemberGetter @Cast("int*") @ByRef IntPointer attributes();

        public native @MemberGetter @Cast("byte*") @ByRef @Name("transform") BytePointer first();

        public int getInt24(int offset) {
            return (first().get(offset) | (first().get(offset+1)<<8) | (first().get(offset+2)<<16));
        };

        public void setInt24(int offset, int num24) {
            BytePointer ptr = first();
            ptr.put((long)(offset+0), (byte)(num24&0xFF));
            ptr.put((long)(offset+1), (byte)((num24>>8)&0xFF));
            ptr.put((long)(offset+2), (byte)((num24>>16)&0xFF));
        };
    };

    //
    @Name("vlr::ConstantDesc")
    public static class ConstantDesc extends Pointer {
        static { Loader.load(); }

        public ConstantDesc(Pointer p) { super(p); };
        public ConstantDesc() { allocate(); };

        private native void allocate();

        public native @MemberGetter @Cast("float*") @ByRef FloatPointer projection();
        public native @MemberGetter @Cast("float*") @ByRef FloatPointer projectionInv();
        public native @MemberGetter @Cast("float*") @ByRef FloatPointer modelview();
        public native @MemberGetter @Cast("float*") @ByRef FloatPointer modelviewInv();
        public native @MemberGetter @Cast("float*") @ByRef FloatPointer modelviewPrev();
        public native @MemberGetter @Cast("float*") @ByRef FloatPointer modelviewPrevInv();
        public native @MemberGetter @Cast("int*") @ByRef IntPointer mdata();
        public native @MemberGetter @Cast("int*") @ByRef IntPointer tdata();
        public native @MemberGetter @Cast("int*") @ByRef IntPointer rdata();
    };

    // 
    @Name("vlr::PipelineStages")
    public static class PipelineStages extends Pointer {
        static { Loader.load(); }

        public PipelineStages(Pointer p) { super(p); };
        public PipelineStages() { allocate(); };

        private native void allocate();

        public native @MemberSetter void vertexShader(@ByRef @StdString String vertexShader);
        public native @MemberSetter void geometryShader(@ByRef @StdString String geometryShader);
        public native @MemberSetter void fragmentShader(@ByRef @StdString String fragmentShader);
        public native @MemberGetter @StdString @ByRef String vertexShader();
        public native @MemberGetter @StdString @ByRef String geometryShader();
        public native @MemberGetter @StdString @ByRef String fragmentShader();
    };

    // 
    @Name("vlr::PipelineCreateInfo")
    public static class PipelineCreateInfo extends Pointer {
        static { Loader.load(); }

        public PipelineCreateInfo(Pointer p) { super(p); };
        public PipelineCreateInfo() { allocate(); };

        private native void allocate();

        public native @MemberGetter @ByRef PipelineStages translucent();
        public native @MemberGetter @ByRef PipelineStages opaque();
        public native @MemberSetter void translucent(@ByRef PipelineStages translucent);
        public native @MemberSetter void opaque(@ByRef PipelineStages opaque);

        public native @MemberGetter @UniPtr PipelineLayout layout();
        public native @MemberGetter @UniPtr Framebuffer framebuffer();
        public native @MemberGetter @UniPtr InstanceSet instanceSet();
        public native @MemberGetter @UniPtr Constants constants();

        public native @MemberSetter void layout(@UniPtr PipelineLayout layout);
        public native @MemberSetter void framebuffer(@UniPtr Framebuffer framebuffer);
        public native @MemberSetter void instanceSet(@UniPtr InstanceSet instanceSet);
        public native @MemberSetter void constants(@UniPtr Constants constants);
    };

    // 
    @Name("vlr::RayTracingCreateInfo")
    public static class RayTracingCreateInfo extends Pointer {
        static { Loader.load(); }

        public RayTracingCreateInfo(Pointer p) { super(p); };
        public RayTracingCreateInfo() { allocate(); };

        private native void allocate();

        public native @MemberGetter @UniPtr PipelineLayout layout();
        public native @MemberGetter @UniPtr Framebuffer framebuffer();
        public native @MemberGetter @UniPtr Acceleration accelerationTop();
        public native @MemberGetter @UniPtr Constants constants();

        public native @MemberSetter void layout(@UniPtr PipelineLayout layout);
        public native @MemberSetter void framebuffer(@UniPtr Framebuffer framebuffer);
        public native @MemberSetter void accelerationTop(@UniPtr Acceleration accelerationTop);
        public native @MemberSetter void constants(@UniPtr Constants constants);
    };


    // 
    @Name("vlr::SetBase")
    public static class SetBase extends Pointer {
        static { Loader.load(); }

        // 
        public SetBase(Pointer p) { super(p); };
        public SetBase(@UniPtr VKt.Driver driver, DataSetCreateInfo info) { allocate(driver, info); };
        public SetBase(@UniPtr VKt.Driver driver) { allocate(driver); };
        public SetBase() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
        private native void allocate(@UniPtr VKt.Driver driver, DataSetCreateInfo info);
    };


    //
    @Name("vlr::AttributeSet")
    public static class AttributeSet extends SetBase {};

    //
    @Name("vlr::BindingSet")
    public static class BindingSet extends SetBase {};

    //
    @Name("vlr::MaterialSet")
    public static class MaterialSet extends SetBase {};

    //
    @Name("vlr::Constants")
    public static class Constants extends SetBase {};

    //
    //@Name("vlr::GeometrySet")
    //public static class GeometrySet extends SetBase {};

    //
    @Name("vlr::InstanceSet")
    public static class InstanceSet extends SetBase {};


    // 
    @Name("vlr::VertexSet")
    public static class VertexSet extends Pointer {
        static { Loader.load(); }

        // 
        public VertexSet(Pointer p) { super(p); };
        public VertexSet(@UniPtr VKt.Driver driver, VertexSetCreateInfo info) { allocate(driver, info); };
        public VertexSet(@UniPtr VKt.Driver driver) { allocate(driver); };
        public VertexSet() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
        private native void allocate(@UniPtr VKt.Driver driver, VertexSetCreateInfo info);
    };

    //
    @Name("vlr::Acceleration")
    public static class Acceleration extends Pointer {
        static { Loader.load(); }

        // 
        public Acceleration(Pointer p) { super(p); };
        public Acceleration(@UniPtr VKt.Driver driver, AccelerationCreateInfo info) { allocate(driver, info); };
        public Acceleration(@UniPtr VKt.Driver driver) { allocate(driver); };
        public Acceleration() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver, AccelerationCreateInfo info);
        private native void allocate(@UniPtr VKt.Driver driver);
    };

    //
    @Name("vlr::Geometry")
    public static class Geometry extends Pointer {
        static { Loader.load(); }

        // 
        public Geometry(Pointer p) { super(p); };
        public Geometry(@UniPtr VertexSet vset, GeometryDesc info) { allocate(vset, info); };
        public Geometry(@UniPtr VertexSet vset) { allocate(vset); };
        public Geometry() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VertexSet vset, GeometryDesc info);
        private native void allocate(@UniPtr VertexSet vset);
    };

    // 
    @Name("vlr::GeometrySet")
    public static class GeometrySet extends Pointer {
        static { Loader.load(); }

        // 
        public GeometrySet(Pointer p) { super(p); };
        public GeometrySet() { allocate(); };
        //public GeometrySet(@UniPtr VKt.Driver driver, DataSetCreateInfo info) { allocate(driver, info); };
        //public GeometrySet(@UniPtr VKt.Driver driver) { allocate(driver); };
        public GeometrySet(@UniPtr VertexSet driver, DataSetCreateInfo info) { allocate(driver, info); };
        public GeometrySet(@UniPtr VertexSet driver) { allocate(driver); };

        // 
        private native void allocate();
        //private native void allocate(@UniPtr VKt.Driver driver);
        //private native void allocate(@UniPtr VKt.Driver driver, DataSetCreateInfo info);
        private native void allocate(VertexSet driver);
        private native void allocate(VertexSet driver, DataSetCreateInfo info);
    };


    //
    @Name("vlr::Background")
    public static class Background extends Pointer {
        static { Loader.load(); }

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
    @Name("vlr::BufferViewSet")
    public static class BufferViewSet extends Pointer {
        static { Loader.load(); }

        // 
        public BufferViewSet(Pointer p) { super(p); };
        public BufferViewSet(@UniPtr VKt.Driver driver) { allocate(driver); };
        public BufferViewSet() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
    };

    //
    @Name("vlr::TextureSet")
    public static class TextureSet extends Pointer {
        static { Loader.load(); }

        // 
        public TextureSet(Pointer p) { super(p); };
        public TextureSet(@UniPtr VKt.Driver driver) { allocate(driver); };
        public TextureSet() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
    };

    //
    @Name("vlr::SamplerSet")
    public static class SamplerSet extends Pointer {
        static { Loader.load(); }

        // 
        public SamplerSet(Pointer p) { super(p); };
        public SamplerSet(@UniPtr VKt.Driver driver) { allocate(driver); };
        public SamplerSet() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
    };
    



    // 
    @Name("vlr::RenderCommand")
    public static class RenderCommand extends Pointer {
        static { Loader.load(); }

        // 
        public RenderCommand(Pointer p) { super(p); };
        public RenderCommand(@UniPtr VKt.Driver driver, RenderCommandCreateInfo info) { allocate(driver, info); };
        public RenderCommand(@UniPtr VKt.Driver driver) { allocate(driver); };
        public RenderCommand() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
        private native void allocate(@UniPtr VKt.Driver driver, RenderCommandCreateInfo info);
    };

    // 
    @Name("vlr::BuildCommand")
    public static class BuildCommand extends Pointer {
        static { Loader.load(); }

        // 
        public BuildCommand(Pointer p) { super(p); };
        public BuildCommand(@UniPtr VKt.Driver driver, BuildCommandCreateInfo info) { allocate(driver, info); };
        public BuildCommand(@UniPtr VKt.Driver driver) { allocate(driver); };
        public BuildCommand() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
        private native void allocate(@UniPtr VKt.Driver driver, BuildCommandCreateInfo info);
    };

    // 
    @Name("vlr::RayTracing")
    public static class RayTracing extends Pointer {
        static { Loader.load(); }

        // 
        public RayTracing(Pointer p) { super(p); };
        public RayTracing(@UniPtr VKt.Driver driver, RayTracingCreateInfo info) { allocate(driver, info); };
        public RayTracing(@UniPtr VKt.Driver driver) { allocate(driver); };
        public RayTracing() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
        private native void allocate(@UniPtr VKt.Driver driver, RayTracingCreateInfo info);
    };

    // 
    @Name("vlr::Rasterization")
    public static class Rasterization extends Pointer {
        static { Loader.load(); }

        // 
        public Rasterization(Pointer p) { super(p); };
        public Rasterization(@UniPtr VKt.Driver driver, PipelineCreateInfo info) { allocate(driver, info); };
        public Rasterization(@UniPtr VKt.Driver driver) { allocate(driver); };
        public Rasterization() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
        private native void allocate(@UniPtr VKt.Driver driver, PipelineCreateInfo info);
    };

    // 
    @Name("vlr::PipelineLayout")
    public static class PipelineLayout extends Pointer {
        static { Loader.load(); }

        // 
        public PipelineLayout(Pointer p) { super(p); };
        public PipelineLayout(@UniPtr VKt.Driver driver) { allocate(driver); };
        public PipelineLayout() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
    };

    // 
    @Name("vlr::Framebuffer")
    public static class Framebuffer extends Pointer {
        static { Loader.load(); }

        // 
        public Framebuffer(Pointer p) { super(p); };
        public Framebuffer(@UniPtr VKt.Driver driver) { allocate(driver); };
        public Framebuffer() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr VKt.Driver driver);
    };


};
