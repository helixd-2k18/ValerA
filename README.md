# ValerA

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

Classic-like batch ray tracing, but based on `VK_KHR_ray_tracing` ideas... vRt reload!

### Planned Features:

- [x] Fully refactored and rewrited code (and shorter)
- [x] Lower-Level API (than JiviX)
- [x] Object and Data oriented
- [x] Partial modularity and faster compilation
- [x] No CMAKE, more direct compilation
- [x] Multiple compute shaders rendering (include Ray Query)
- [x] More and efficient ray-tracing
- [x] OptiX Denoiser support (but obj example only)
- [x] Static Buffer Views set
- [ ] Dynamic Buffer Views set
- [ ] More performance and, probably, multi-threading
- [ ] DirectX 12 Ultimate support and HLSL (under consideration)
- [ ] RDNA-2 support
- [ ] ...


### Shaders Features: 

- [x] Global Illumination
- [x] World Space Reflections
- [x] Multiple textures support
- [x] Unified GLSL code
- [x] Accessors and Bindings sets
- [ ] HLSL and DXIL support


### Why OBJ instead of GLTF?

GLTF is much complex format, very complex for support and Debug. Do not misunderstand me, I have already spent my energy, the monetary equivalent of which exceeds the permissible limits in the amount of over 50$... and GLTF support also requires f&cking with dances, and this is also 10$ in a similar equivalent... now my strength is enough a maximum of 1$ per day, which is extremely small. I already barely met the monthly limit for the first version (for a library or framework, this is just an early alpha)... It's just that OBJ is easier to maintain and debug, and costs about only 1$ per day of work...

## Additional Links

- [VKt/VKh](https://github.com/world8th/vkt), simpler and unified helpers and mini-framework for Vulkan API... **Required** for the project!
- [MineVKR](https://github.com/hyperearth/MineVKR), spin-off project for add RTX ray tracing for Minecraft (Java Edition)...
- [My Discord](https://discord.gg/NqjBJsG), but currently we have no focus for them...
- [My Puzzle](https://vk.cc/afiR3v) for psychologists, psychoanalysts, psychotherapists and students ...

## Tech Spec

```MD
- Architecture    : [ARC:F2020H:XT] (Connor Lake)
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

Date:[08.08.2020]:[helix.d@IMN]
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


## P.S. Of Turing

Архитектура Turing оказалась крайне ебанутой по своей сути. Потому жду NVIDIA Ampere (RTX-3070 или AMD RDNA-2 (RX 6900 XT). Проблема в особенностях Acceleration Structure, позволяющей работать лишь в монопольном режиме. 
