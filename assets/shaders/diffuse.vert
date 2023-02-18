#version 430
// glslangValidator -l diffuse.vert
// #extension GL_GOOGLE_include_directive : require
#extension GL_ARB_shading_language_include : require

#include "/lib/VertexData.glsl"

uniform mat4 worldFromObject; // Model
uniform mat4 viewFromWorld; // View
uniform mat4 projectionFromView; // Projection

layout(location = 0) in vec3 objectPosition;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 objectNormal;
layout(location = 3) in vec4 color;
layout(location = 4) in vec4 custom;

out VertexData v;

void main() {
  v.objectPosition = objectPosition;
  v.worldPosition = vec3(worldFromObject * vec4(objectPosition, 1));
  v.objectNormal = objectNormal;
  v.worldNormal = mat3(transpose(inverse(worldFromObject))) * objectNormal;      
  v.texCoord = texCoord;
  v.color = color;
  v.custom = custom;
  gl_Position = projectionFromView * viewFromWorld * vec4(v.worldPosition, 1); // v.projectionPosition
}
