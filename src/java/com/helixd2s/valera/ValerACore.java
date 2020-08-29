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

        public MaterialUnit(Pointer p) {
            super(p);
        }
        public MaterialUnit() {
            allocate();
        }

        private native void allocate();
    };

    // 
    @Name("vlr::AccelerationCreateInfo")
    public static class AccelerationCreateInfo extends Pointer {
        static { Loader.load(); }

        public AccelerationCreateInfo(Pointer p) { super(p); };
        public AccelerationCreateInfo() { allocate(); };

        private native void allocate();
    };
    

    //
    @Name("vlr::Acceleration")
    public static class Acceleration extends Pointer {
        static { Loader.load(); }

        // 
        public Acceleration(Pointer p) { super(p); };
        public Acceleration(@UniPtr Driver driver, AccelerationCreateInfo info) { allocate(driver, info); };
        public Acceleration(@UniPtr Driver driver) { allocate(driver); };
        public Acceleration() { allocate(); };

        // 
        private native void allocate();
        private native void allocate(@UniPtr Driver driver, AccelerationCreateInfo info);
        private native void allocate(@UniPtr Driver driver);
    };

};
