# ValerA

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

Classic-like batch ray tracing, but based on `VK_KHR_ray_tracing` ideas... vRt reload! 


### Planned Features: 

- Fully refactored and rewrited code (and shorter)
- More performance and, probably, multi-threading
- Lower-Level API (than JiviX)
- Object and Data oriented
- Partial modularity and faster compilation
- No CMAKE, more direct compilation
- Multiple compute shaders rendering (include Ray Query)
- More and efficient ray-tracing 
- DirectX 12 Ultimate support and HLSL (under consideration)
- RDNA-2 support
- ...


## Additional Links

- [VKt/VKh](https://github.com/world8th/vkt), simpler and unified helpers and mini-framework for Vulkan API... **Required** for the project!
- [MineVKR](https://github.com/hyperearth/MineVKR), spin-off project for add RTX ray tracing for Minecraft (Java Edition)...
- [My Discord](https://discord.gg/NqjBJsG), but currently we have no focus for them... 
- [My Puzzle](https://vk.cc/afiR3v) for psychologists, psychoanalysts, psychotherapists and students ...


## Tech Spec

```MD
- Architecture    : [ARC:F2020H:XT]
- Portfolio       : Work in Progress
- License         : Apache-2.0 (Currently)
- Execute Day     : [2021:Q1]
- Commercial      : Under Consideration
- Project Type    : Experimental, Personal
- Path-Tracing    : Diffuse noiseless (almost), reflection...
- Target Games    : Minecraft Java Edition, Neverball...
- Graphical API   : Vulkan API (VK_KHR_ray_tracing)
- General To Do   : Add Transform Feedback support
- Used Libraries  : Vulkan-HPP, VKt/VKh, 
- Replacement Of  : Project "vRt" ("engine", 2018)
- Shaders Version : v2.0

Date:[27.07.2020]:[helix.d@IMN]
```


### About Minecraft Mod Idea... 

- Vulkan API can import OpenGL buffer data only using copy, OpenGL can import memory directly from Vulkan API. 
- Minecraft can use Vulkan API using JNI, JavaCPP, LWJGL-3... 
- Always communication using LONG type in Java and uint64_t in C++ 
- Java may have conflicts with `VkDeviceOrHostAddressKHR` or `VkDeviceOrHostAddressConstKHR` due same LONG type... 
- Any native pointers in Java are LONG type... 


### Used Symbolic Links!

Clone with `git clone -c core.symlinks=true https://github.com/hyperearth/LancER.git --recursive`


## Authors P.S.

- helix.d@IMN
- RTX@NVIDIA

