package com.helixd2s.valera;

import org.bytedeco.javacpp.*;
import org.bytedeco.javacpp.annotation.*;
import org.bytedeco.javacpp.tools.*;
import org.bytedeco.javacpp.indexer.*;
import java.lang.annotation.*;

// "jniJiviXBase", "JiviX"

@Platform(include = {
        "./include/vkt3/fw.hpp",
        "./include/stdafx.h",
}, link = {"vulkan-1", "glfw"}, define = {"ENABLE_OPENGL_INTEROP", "WIN32", "OS_WIN", "VK_ENABLE_BETA_EXTENSIONS", "VK_USE_PLATFORM_WIN32_KHR", "SHARED_PTR_NAMESPACE std", "UNIQUE_PTR_NAMESPACE std", "VKT_ENABLE_GLFW_SUPPORT"})
@Name("") //
public class VKt implements InfoMapper {
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
        infoMap.put(new Info("std::shared_ptr").skip().annotations("@UniPtr"));
    };

    // Pointer for xvk::Device
    @Name("xvk::Device")
    public static class Device extends Pointer {
        static { Loader.load(); }

        public Device(Pointer p) {
            super(p);
        }
        public Device() {
            allocate();
        }
        private native void allocate();
    };

    // Pointer for xvk::Instance
    @Name("xvk::Instance")
    public static class Instance extends Pointer {
        static { Loader.load(); }

        public Instance(Pointer p) {
            super(p);
        }
        public Instance() {
            allocate();
        }
        private native void allocate();
    };

    // 
    @Name("vkt::VmaMemoryInfo")
    public static class VmaMemoryInfo extends Pointer {
        static { Loader.load(); }

        public VmaMemoryInfo(Pointer p) {
            super(p);
        }
        public VmaMemoryInfo() {
            allocate();
        }

        //
        private native void allocate();

        //
        public native @ByRef @UniPtr Device getDeviceDispatch();
        public native @ByRef @UniPtr Instance getInstanceDispatch();

        //
        public native @ByRef VmaMemoryInfo setDeviceDispatch(@UniPtr Device device);
        public native @ByRef VmaMemoryInfo setInstanceDispatch(@UniPtr Instance instance);
    };

    // 
    @Name("vkt::MemoryAllocationInfo")
    public static class MemoryAllocationInfo extends Pointer {
        static { Loader.load(); }

        public MemoryAllocationInfo(Pointer p) {
            super(p);
        }
        public MemoryAllocationInfo() {
            allocate();
        }

        private native void allocate();
    };

    // 
    @Name("vkt::ImageAllocation")
    public static class ImageAllocation extends Pointer {
        static { Loader.load(); }

        public ImageAllocation(Pointer p) {
            super(p);
        }
        public ImageAllocation(@UniPtr ImageAllocation alloc) {
            allocate(alloc);
        }
        public ImageAllocation(@Cast("vkh::VkImageCreateInfo*") long createInfoAddress, MemoryAllocationInfo allocationInfoAddress) {
            allocate(createInfoAddress, allocationInfoAddress);
        }
        public ImageAllocation() {
            allocate();
        }

        private native void allocate(@UniPtr ImageAllocation alloc);
        private native void allocate(@Cast("vkh::VkImageCreateInfo*") long createInfoAddress, MemoryAllocationInfo allocationInfoAddress);
        private native void allocate();

        //private native @UniPtr ImageAllocation uniPtr();

        public native int getGLImage();
        public native int getGLMemory();
    };

    // 
    @Name("vkt::VmaImageAllocation")
    public static class VmaImageAllocation extends Pointer {
        static { Loader.load(); }

        public VmaImageAllocation(Pointer p) {
            super(p);
        }
        public VmaImageAllocation() {
            allocate();
        }
        public VmaImageAllocation(@UniPtr ImageAllocation alloc) {
            allocate(alloc);
        }
        public VmaImageAllocation(@Cast("VmaAllocator*") long vmaAllocator, @Cast("vkh::VkImageCreateInfo*") long createInfo, @Cast("vkt::VmaMemoryInfo*") long memInfoPtr) {
            allocate(vmaAllocator, createInfo, memInfoPtr);
        }
        public VmaImageAllocation(@Cast("VmaAllocator*") long vmaAllocator, @Cast("vkh::VkImageCreateInfo*") long createInfo, VmaMemoryInfo memInfo) {
            allocate(vmaAllocator, createInfo, memInfo);
        }

        private native void allocate();
        private native void allocate(@UniPtr ImageAllocation alloc);
        private native void allocate(@Cast("VmaAllocator*") long vmaAllocator, @Cast("vkh::VkImageCreateInfo*") long createInfo, @Cast("vkt::VmaMemoryInfo*") long memInfoPtr);
        private native void allocate(@Cast("VmaAllocator*") long vmaAllocator, @Cast("vkh::VkImageCreateInfo*") long createInfo, VmaMemoryInfo memInfo);

        //private native @UniPtr ImageAllocation uniPtr();

        public native int getGLImage();
        public native int getGLMemory();
    };

    // 
    @Name("vkt::BufferAllocation")
    public static class BufferAllocation extends Pointer {
        static { Loader.load(); }

        public BufferAllocation(Pointer p) {
            super(p);
        }
        public BufferAllocation(@UniPtr BufferAllocation alloc) {
            allocate(alloc);
        }
        public BufferAllocation(@Cast("vkh::VkBufferCreateInfo*") long createInfoAddress, MemoryAllocationInfo allocationInfoAddress) {
            allocate(createInfoAddress, allocationInfoAddress);
        }
        public BufferAllocation() {
            allocate();
        }

        private native void allocate(@UniPtr BufferAllocation alloc);
        private native void allocate(@Cast("vkh::VkBufferCreateInfo*") long createInfoAddress, MemoryAllocationInfo allocationInfoAddress);
        private native void allocate();

        //private native @UniPtr BufferAllocation uniPtr();
    };

    // 
    @Name("vkt::VmaBufferAllocation")
    public static class VmaBufferAllocation extends Pointer {
        static { Loader.load(); }

        public VmaBufferAllocation(Pointer p) {
            super(p);
        }
        public VmaBufferAllocation() {
            allocate();
        }
        public VmaBufferAllocation(@UniPtr BufferAllocation alloc) {
            allocate(alloc);
        }
        public VmaBufferAllocation(@Cast("VmaAllocator*") long vmaAllocator, @Cast("vkh::VkBufferCreateInfo*") long createInfo, @Cast("vkt::VmaMemoryInfo*") long memInfoPtr) {
            allocate(vmaAllocator, createInfo, memInfoPtr);
        }
        public VmaBufferAllocation(@Cast("VmaAllocator*") long vmaAllocator, @Cast("vkh::VkBufferCreateInfo*") long createInfo, VmaMemoryInfo memInfo) {
            allocate(vmaAllocator, createInfo, memInfo);
        }

        private native void allocate();
        private native void allocate(@UniPtr BufferAllocation alloc);
        private native void allocate(@Cast("VmaAllocator*") long vmaAllocator, @Cast("vkh::VkBufferCreateInfo*") long createInfo, @Cast("vkt::VmaMemoryInfo*") long memInfoPtr);
        private native void allocate(@Cast("VmaAllocator*") long vmaAllocator, @Cast("vkh::VkBufferCreateInfo*") long createInfo, VmaMemoryInfo memInfo);
        //private native @UniPtr BufferAllocation uniPtr();
    };
    
    //
    public static native @Name("vkt::initializeGL")
    void initializeGL(@Cast("GLFWglproc(*)(const char*)") long GetProcAddress);

    // Required Linked
    //public static native @Name("vkt::initializeGL")
    //void initializeGL();

    //
    public static native @Name("vkt::createSemaphore")
    void createSemaphore(Device device, @Cast("VkSemaphore*") long[] vkSemaphore, @Cast("unsigned*") int[] glSemaphore, @Cast("const void*") long pNext);

    //
    public static native @Name("vkt::submitCmd")
    void submitCmd(Device device, @Cast("VkQueue") long queue, @StdVector @Cast("VkCommandBuffer*") long[] cmds, @Cast("vkh::VkSubmitInfo*") long smbi);

    //
    public static native @Name("vkt::submitCmdAsync")
    void submitCmdAsync(Device device, @Cast("VkQueue") long queue, @StdVector @Cast("VkCommandBuffer*") long[] cmds, @Cast("vkh::VkSubmitInfo*") long smbi);

    //
    public static native @Name("vkt::createCommandBuffer") @Cast("VkCommandBuffer")
    long createCommandBuffer(Device device, @Cast("VkCommandPool") long commandPool, byte secondary, byte once);

    //
    public static native @Name("vkt::createCompute") @Cast("VkPipeline")
    long createCompute(Device device, @StdString String path, @Cast("VkPipelineLayout") long layout, @Cast("VkPipelineCache") long cache, @Cast("uint32_t") int subgroupSize);

    //
    public static native @Name("vkt::createCompute") @Cast("VkPipeline")
    long createCompute(Device device, @StdVector @Cast("uint32_t*") int[] code, @Cast("VkPipelineLayout") long layout, @Cast("VkPipelineCache") long cache, @Cast("uint32_t") int subgroupSize);


    //
    @Name("vkt::ImageRegion") //
    public static class ImageRegion extends Pointer {
        static { Loader.load(); }

        public ImageRegion() {
            allocate();
        }
        public ImageRegion(Pointer p) {
            super(p);
        }
        public ImageRegion(ImageRegion b) {
            allocate(b);
        }
        public ImageRegion(@UniPtr ImageAllocation alloc, @Cast("vkh::VkImageViewCreateInfo*") long info, @Cast("VkImageLayout") int layout) { allocate(alloc, info, layout); }
        public ImageRegion(@UniPtr VmaImageAllocation alloc, @Cast("vkh::VkImageViewCreateInfo*") long info, @Cast("VkImageLayout") int layout) { allocate(alloc, info, layout); }

        private native void allocate();
        private native void allocate(ImageRegion b);
        private native void allocate(@UniPtr ImageAllocation alloc, @Cast("vkh::VkImageViewCreateInfo*") long info, @Cast("VkImageLayout") int layout);
        private native void allocate(@UniPtr VmaImageAllocation alloc, @Cast("vkh::VkImageViewCreateInfo*") long info, @Cast("VkImageLayout") int layout);

        //
        public native int getGLImage();
        public native int getGLMemory();

        //
        public native @Cast("uintptr_t") long getDescriptorPtr();
    };

    @Name("vkt::VectorBase") // TODO: Untyped Vector in Native
    public static class Vector extends Pointer {
        static { Loader.load(); }

        //
        public Vector() { super(); }
        public Vector(Pointer p) {
            super(p);
        }
        public Vector(@UniPtr VmaBufferAllocation a, long offset, long size, long stride) { allocate(a, offset, size, stride); }
        public Vector(@UniPtr BufferAllocation a, long offset, long size, long stride) { allocate(a, offset, size, stride); }

        //
        protected native void allocate();
        protected native void allocate(@UniPtr VmaBufferAllocation a, long offset, long size, long stride);
        protected native void allocate(@UniPtr BufferAllocation a, long offset, long size, long stride);

        // Java have NOT support `VkDeviceOrHostAddressKHR` or `VkDeviceOrHostAddressConstKHR`, and become rude var... (DeRMo!)
        public native @Cast("uintptr_t") long deviceAddress();
        public native @Cast("uintptr_t") long address(); // For Pointer Passes
        public native @Cast("uintptr_t") long mapv(long ptr);
        public native @Name("operator=") @ByRef Vector put(@ByRef Vector x);
        
        //
        public native long size();
        public native long range();

        //
        public native int getGL();
        public native int getGLBuffer();
        public native int getGLMemory();

        //
        public native @Cast("uintptr_t") long getDescriptorPtr();
    };

    // For JAVA only
    @Name("vkt::Vector<int8_t>") // int8_t default
    public static class ByteVector extends Vector {
        static { Loader.load(); }

        //
        public ByteVector() { super(); }
        public ByteVector(Pointer p) {
            super(p);
        }
        public ByteVector(@UniPtr VmaBufferAllocation a, long offset, long size) { super(a, offset, size, 1); }
        public ByteVector(@UniPtr BufferAllocation a, long offset, long size) { super(a, offset, size, 1); }

        // Indexer for Data
        //public ByteBufferIndexer getIndexer() {
        //    return new ByteBufferIndexer(this.data().asBuffer());
        //}

        //
        @Name("operator=") public native @ByRef ByteVector put(@ByRef ByteVector x);
        @Name("operator[]") public native @ByRef BytePointer at(long n);

        // map buffer data
        public native BytePointer mapped();
        public native BytePointer mapped(long ptr);
        public native BytePointer map();
        public native BytePointer map(long ptr);
        public native BytePointer data();
        public native BytePointer data(long ptr);
    };

    // FOR KOTLIN AND NATIVE!
    @Name("vkt::Vector<uint8_t>") // uint8_t version (C++)
    public static class UByteVector extends Vector {
        static { Loader.load(); }

        //
        public UByteVector(Pointer p) {
            super(p);
        }
        public UByteVector() { super(); }
        public UByteVector(@UniPtr VmaBufferAllocation a, long offset, long size) { super(a, offset, size, 1); }
        public UByteVector(@UniPtr BufferAllocation a, long offset, long size) { super(a, offset, size, 1); }

        // Indexer for Data
        //public UByteBufferIndexer getIndexer() {
        //    return new UByteBufferIndexer(this.data().asBuffer());
        //}

        //
        @Name("operator=") public native @ByRef UByteVector put(@ByRef UByteVector x);
        @Name("operator[]") public native @Cast("uint8_t*") @ByRef BytePointer at(long n);

        // map buffer data
        public native @Cast("uint8_t*")BytePointer mapped();
        public native @Cast("uint8_t*")BytePointer mapped(long ptr);
        public native @Cast("uint8_t*")BytePointer map();
        public native @Cast("uint8_t*")BytePointer map(long ptr);
        public native @Cast("uint8_t*")BytePointer data();
        public native @Cast("uint8_t*")BytePointer data(long ptr);
    };

    // For JAVA only
    @Name("vkt::Vector<int16_t>") // int8_t default
    public static class ShortVector extends Vector {
        static { Loader.load(); }

        //
        public ShortVector(Pointer p) {
            super(p);
        }
        public ShortVector() { super(); }
        public ShortVector(@UniPtr VmaBufferAllocation a, long offset, long size) { super(a, offset, size, 2); }
        public ShortVector(@UniPtr BufferAllocation a, long offset, long size) { super(a, offset, size, 2); }

        // Indexer for Data
        //public ShortBufferIndexer getIndexer() {
        //    return new ShortBufferIndexer(this.data().asBuffer());
        //}

        //
        @Name("operator=") public native @ByRef ShortVector put(@ByRef ShortVector x);
        @Name("operator[]") public native @ByRef ShortPointer at(long n);

        // map buffer data
        public native ShortPointer mapped();
        public native ShortPointer mapped(long ptr);
        public native ShortPointer map();
        public native ShortPointer map(long ptr);
        public native ShortPointer data();
        public native ShortPointer data(long ptr);
    };

    // FOR KOTLIN AND NATIVE!
    @Name("vkt::Vector<uint16_t>") // uint8_t version (C++)
    public static class UShortVector extends Vector {
        static { Loader.load(); }

        //
        public UShortVector(Pointer p) {
            super(p);
        }
        public UShortVector() { super(); }
        public UShortVector(@UniPtr VmaBufferAllocation a, long offset, long size) { super(a, offset, size, 2); }
        public UShortVector(@UniPtr BufferAllocation a, long offset, long size) { super(a, offset, size, 2); }

        // Indexer for Data
        //public UShortBufferIndexer getIndexer() {
        //    return new UShortBufferIndexer(this.data().asBuffer());
        //}

        //
        @Name("operator=") public native @ByRef UShortVector put(@ByRef UShortVector x);
        @Name("operator[]") public native @Cast("uint16_t*") @ByRef ShortPointer at(long n);

        // map buffer data
        public native @Cast("uint16_t*") ShortPointer mapped();
        public native @Cast("uint16_t*") ShortPointer mapped(long ptr);
        public native @Cast("uint16_t*") ShortPointer map();
        public native @Cast("uint16_t*") ShortPointer map(long ptr);
        public native @Cast("uint16_t*") ShortPointer data();
        public native @Cast("uint16_t*") ShortPointer data(long ptr);
    };

    // For JAVA only
    @Name("vkt::Vector<int32_t>") // int8_t default
    public static class IntVector extends Vector {
        static { Loader.load(); }

        //
        public IntVector(Pointer p) {
            super(p);
        }
        public IntVector() { super(); }
        public IntVector(@UniPtr VmaBufferAllocation a, long offset, long size) { super(a, offset, size, 4); }
        public IntVector(@UniPtr BufferAllocation a, long offset, long size) { super(a, offset, size, 4); }

        // Indexer for Data
        //public IntBufferIndexer getIndexer() {
        //    return new IntBufferIndexer(this.data().asBuffer());
        //}

        //
        @Name("operator=") public native @ByRef IntVector put(@ByRef IntVector x);
        @Name("operator[]") public native @ByRef IntPointer at(long n);

        // map buffer data
        public native IntPointer mapped();
        public native IntPointer mapped(long ptr);
        public native IntPointer map();
        public native IntPointer map(long ptr);
        public native IntPointer data();
        public native IntPointer data(long ptr);
    };

    // FOR KOTLIN AND NATIVE!
    @Name("vkt::Vector<uint32_t>") // uint8_t version (C++)
    public static class UIntVector extends Vector {
        static { Loader.load(); }

        //
        public UIntVector(Pointer p) {
            super(p);
        }
        public UIntVector() { super(); }
        public UIntVector(@UniPtr VmaBufferAllocation a, long offset, long size) { super(a, offset, size, 4); }
        public UIntVector(@UniPtr BufferAllocation a, long offset, long size) { super(a, offset, size, 4); }

        // Indexer for Data
        //public UIntBufferIndexer getIndexer() {
        //    return new UIntBufferIndexer(this.data().asBuffer());
        //}

        //
        @Name("operator=") public native @ByRef UIntVector put(@ByRef UIntVector x);
        @Name("operator[]") public native @Cast("uint32_t*") @ByRef IntPointer at(long n);

        // map buffer data
        public native @Cast("uint32_t*") IntPointer mapped();
        public native @Cast("uint32_t*") IntPointer mapped(long ptr);
        public native @Cast("uint32_t*") IntPointer map();
        public native @Cast("uint32_t*") IntPointer map(long ptr);
        public native @Cast("uint32_t*") IntPointer data();
        public native @Cast("uint32_t*") IntPointer data(long ptr);
    };

    // Universal
    @Name("vkt::Vector<float>") // int8_t default
    public static class FloatVector extends Vector {
        static { Loader.load(); }

        //
        public FloatVector(Pointer p) {
            super(p);
        }
        public FloatVector() { super(); }
        public FloatVector(@UniPtr VmaBufferAllocation a, long offset, long size) { super(a, offset, size, 4); }
        public FloatVector(@UniPtr BufferAllocation a, long offset, long size) { super(a, offset, size, 4); }

        // Indexer for Data
        //public FloatBufferIndexer getIndexer() {
        //    return new FloatBufferIndexer(this.data().asBuffer());
        //}

        //
        @Name("operator=") public native @ByRef FloatVector put(@ByRef FloatVector x);
        @Name("operator[]") public native @ByRef FloatPointer at(long n);

        // map buffer data
        public native FloatPointer mapped();
        public native FloatPointer mapped(long ptr);
        public native FloatPointer map();
        public native FloatPointer map(long ptr);
        public native FloatPointer data();
        public native FloatPointer data(long ptr);
    };

    // 
    @Name("vkt::GPUFramework")
    public static class Driver extends Pointer {
        static { Loader.load(); }

        public Driver(Pointer p) {
            super(p);
        }
        public Driver() {
            allocate();
        }

        private native void allocate();
    };
    
    
};

