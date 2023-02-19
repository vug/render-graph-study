#version 430
// #extension GL_GOOGLE_include_directive : require
#extension GL_ARB_shading_language_include : require

#include "/lib/VertexData.glsl"
in VertexData v;
#include "/lib/SceneUniforms.glsl"

uniform vec4 u_Color = vec4(1.0, 1.0, 1.0, 1.0);
uniform float specularCoeff = 32.0f;

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

#include "/lib/lights/PointLights.glsl"
#include "/lib/lights/DirectionalLights.glsl"
#include "/lib/lights/HemisphericalLight.glsl"
#include "/lib/lights/AmbientLight.glsl"

layout (location = 0) out vec4 outColor;

void main() {
  vec3 wNormal = normalize(v.worldNormal);
  vec3 light = vec3(0);
  for (int i = 0; i < numPointLights; i++)
    light += illuminate(pointLights[i], v.worldPosition, wNormal, eyePos, specularCoeff);
  for (int i = 0; i < numDirectionalLights; i++)
    light += illuminate(directionalLights[i], v.worldPosition, wNormal, eyePos, specularCoeff);
  light += illuminate(hemisphericalLight, wNormal);
  light += illuminate(ambientLight);

  vec3 color = u_Color.rgb * light;
  outColor = vec4(color, 1);
}