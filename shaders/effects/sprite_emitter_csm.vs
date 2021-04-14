LimitlessEngine::GLSL_VERSION
LimitlessEngine::Extensions
LimitlessEngine::Settings
LimitlessEngine::MaterialType
LimitlessEngine::EmitterType

#include "glsl/material.glsl"
#include "glsl/scene.glsl"

layout(location = 0) in vec3 position;
#ifdef InitialColor_MODULE
    layout(location = 1) in vec4 color;
    out vec4 fs_color;
#endif
#ifdef InitialRotation_MODULE
    layout(location = 2) in vec3 rotation;
    out vec3 fs_rotation;
#endif
#ifdef InitialVelocity_MODULE
    layout(location = 3) in vec3 velocity;
    out vec3 fs_velocity;
#endif
#ifdef Lifetime_MODULE
    layout(location = 4) in float lifetime;
    out float fs_lifetime;
#endif
#ifdef InitialSize_MODULE
    layout(location = 5) in vec3 size;
    out vec3 fs_size;
#endif
#ifdef SubUV_MODULE
    layout(location = 6) in vec4 subUV;
    out vec4 fs_subUV;
#endif
#ifdef CustomMaterial_MODULE
    layout(location = 7) in vec4 properties;
    out vec4 fs_properties;
#endif

uniform mat4 light_space;

void main()
{
	gl_Position = light_space * vec4(position, 1.0);

	#ifdef InitialSize_MODULE
	    gl_PointSize = light_space[1][1] * size.x / gl_Position.w;
	#else
	    gl_PointSize = light_space[1][1] * 16.0 / gl_Position.w;
	#endif

    #ifdef InitialColor_MODULE
        fs_color = color;
    #endif

    #ifdef InitialRotation_MODULE
        fs_rotation = rotation;
    #endif

    #ifdef InitialVelocity_MODULE
        fs_velocity = velocity;
    #endif

    #ifdef Lifetime_MODULE
        fs_lifetime = lifetime;
    #endif

    #ifdef InitialSize_MODULE
        fs_size = size;
    #endif

    #ifdef SubUV_MODULE
        fs_subUV = subUV;
    #endif

    #ifdef CustomMaterial_MODULE
        fs_properties = properties;
    #endif
}
