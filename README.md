# C++ Graphics Engine

### The project is cross-platform 3D graphics engine, focused on high-performance, low-overhead rendering with modern OpenGL.

- Features:
    - State caching
    - ShaderCompiler + shader program introspection
    - Indexed buffers auto binding
    - Texture unit auto binding
    - Buffer streaming: Orphaning, Unsynchonized, Persistent, Coherent, + fences
    - Immutable storage support
    - Named buffer objects support
    - Bindless textures
    - Persistent/Coherent mapping + TripleBuffering + Explicit Synchronization

- Material System
  - Albedo
  - Specular
  - Normal
  - Emissive color
  - Emissive mask
  - Blend mask
  - Roughness
  - Metalness
  + Blending: Opaque, Translucent, Additive, Modulate
  + Shading type: Lit, Unlit
  + Shading models: Phong, Blinn-Phong
  + Custom materials via shader code
  
  ![](screenshots/materials.png)
  
- Dynamic Direct Lighting
- Rigged Model Animations

![me](screenshots/model_loader.gif)

- Post Processing: HDR, tone mapping, gamma correction, bloom, vignette, tone shading, FXAA

![](screenshots/tone_shading.png)

Sponza model

![](screenshots/sponza.png)

# Dependencies
- glfw3
- glew
- glm
- assimp
- stb_image

# TODO
- text rendering (screen space + 3d)
- instanced rendering + multidraw extensions support, batching draw optimizations
- material layering
- frustum/occlusion culling
- cascade shadow maps for directional lights
- point cube shadow maps for point lights 
- flexible particle system effects
- skinning in compute shader (as feature if supported) 
- decals projection (+ material influence on base material)
- deferred pipeline configuration
- forward+ pipilne for huge amount of lights
- custom cubemaps
- light probes reflections
- raytracing/pathtracing
