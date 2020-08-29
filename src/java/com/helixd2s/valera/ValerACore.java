package com.helixd2s.valera;

import org.bytedeco.javacpp.*;
import org.bytedeco.javacpp.annotation.*;
import org.bytedeco.javacpp.indexer.*;

// "jniJiviXBase", "JiviX"

// TODO: Wrap CORE Instead Of External Native... (i.e. reduce layers count)
@Platform(include = {
        "./include/GLFW/glfw3.h",
        "./include/vkt3/fw.hpp",
}, link = {"vulkan-1", "glfw", "glbinding", "glbinding-aux"}, define = {"ENABLE_OPENGL_INTEROP", "WIN32", "OS_WIN", "VK_ENABLE_BETA_EXTENSIONS", "VK_USE_PLATFORM_WIN32_KHR", "VMA_IMPLEMENTATION", "SHARED_PTR_NAMESPACE std", "UNIQUE_PTR_NAMESPACE std", "VKT_ENABLE_GLFW_SUPPORT"})
@Name("") //
public class ValerACore extends Pointer {
    static { Loader.load(); }


    @Name("vlj::Driver")
    public static class Driver extends Pointer {
        static { Loader.load(); }

        public Driver(Pointer p) {
            super(p);
        }
        public Driver() {
            allocate();
        }
        public Driver(@SharedPtr VKt.Driver object) {
            allocate(object);
        }

        private native void allocate();
        private native void allocate(@SharedPtr VKt.Driver object);

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
        public native @SharedPtr VKt.Device getDeviceDispatch();
        public native @SharedPtr VKt.Instance getInstanceDispatch();

        //
        public native long memoryAllocationInfoPtr();
    };
    
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
    }
    
};
