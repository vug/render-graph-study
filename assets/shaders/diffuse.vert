#version 430
#extension GL_ARB_shading_language_include : require

#include "/lib/VertexData.glsl"

uniform mat4 worldFromObject; // Model
uniform mat4 viewFromWorld; // View
uniform mat4 projectionFromView; // Projection

layout(location = 0) in vec3 objectPosition;

out VertexData v;

void main() {
    v.worldPosition = vec3(worldFromObject * vec4(objectPosition, 1));
    v.worldNormal = mat3(transpose(inverse(worldFromObject))) * objectPosition;      
    gl_Position = projectionFromView * viewFromWorld * vec4(v.worldPosition, 1); // v.projectionPosition
}
