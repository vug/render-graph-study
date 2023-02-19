#version 430
// #extension GL_GOOGLE_include_directive : require
#extension GL_ARB_shading_language_include : require

#include "/lib/VertexData.glsl"

in VertexData v;

uniform vec4 u_Color = vec4(1.0, 1.0, 1.0, 1.0);

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
    vec3 diffuseLight = vec3(0);
    for (int i = 0; i < numPointLights; i++)
      diffuseLight += illuminate(pointLights[i], v.worldPosition, wNormal);
    for (int i = 0; i < numPointLights; i++)
      diffuseLight += illuminate(directionalLights[i], wNormal);
    diffuseLight += illuminate(hemisphericalLight, wNormal);
    diffuseLight += illuminate(ambientLight);
    vec3 diffuse = u_Color.rgb * diffuseLight;
    outColor = vec4(diffuse, 1);
}