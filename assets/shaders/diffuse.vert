#version 430
// glslangValidator -l diffuse.vert
// #extension GL_GOOGLE_include_directive : require
#extension GL_ARB_shading_language_include : require

#include "/lib/DefaultVertexAttributes.glsl"
#include "/lib/VertexData.glsl"
#include "/lib/SceneUniforms.glsl"

out VertexData v;

void main() {
  v = fillVertexData(worldFromObject, objectPosition, objectNormal, texCoord, color, custom);
  gl_Position = projectionFromView * viewFromWorld * vec4(v.worldPosition, 1); // v.projectionPosition
}
