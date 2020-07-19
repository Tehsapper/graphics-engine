GraphicsEngine::GLSL_VERSION
GraphicsEngine::Extensions

GraphicsEngine::MaterialType

#include "glsl/input_data_fs.glsl"
#include "glsl/material.glsl"

#ifdef MATERIAL_LIT
    #include "glsl/light_computation.glsl"
#endif

out vec4 color;

void main()
{
    #include "glsl/material_variables.glsl"

    // custom material changes

    // computing final color
    vec4 fragment_color = vec4(1.0);
    #include "glsl/material_computation.glsl"

	color = fragment_color;
}