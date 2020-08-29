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

};
