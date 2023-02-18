#version 430
// #extension GL_GOOGLE_include_directive : require
#extension GL_ARB_shading_language_include : require

#include "/lib/VertexData.glsl"

in VertexData v;

uniform vec4 u_Color = vec4(1.0, 1.0, 1.0, 1.0);

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

// POINT_LIGHT

#define MAX_POINT_LIGHTS 8
uniform int numPointLights;
struct PointLight {
  vec3 position;
  vec3 color;
  float intensity;
};
uniform PointLight[MAX_POINT_LIGHTS] pointLights;

vec3 illuminate(PointLight light, vec3 position, vec3 normal) {
  vec3 r = position - light.position;
  float r2 = dot(r, r);
  vec3 lightDir = normalize(r);
  return light.intensity * light.color * max(dot(-lightDir, normal), 0) / r2;
}

// DIRECTIONAL_LIGHT

#define MAX_DIRECTIONAL_LIGHTS 8
uniform int numDirectionalLights;
struct DirectionalLight {
  vec3 direction;
  vec3 color;
  float intensity;
};
uniform DirectionalLight[MAX_DIRECTIONAL_LIGHTS] directionalLights;

vec3 illuminate(DirectionalLight light, vec3 normal) {
  return light.intensity * light.color * max(dot(-light.direction, normal), 0);
}

// HEMISPHERICAL LIGHT

struct HemisphericalLight {
  vec3 northColor;
  vec3 southColor;
  float intensity;
};
uniform HemisphericalLight hemisphericalLight;

vec3 illuminate(HemisphericalLight light, vec3 normal) {
  const vec3 northDir = vec3(0, 1, 0);
  float m = dot(northDir, normal);
  vec3 lightColor = mix(light.southColor, light.northColor, map(m, -1, 1, 0, 1));
  return light.intensity * lightColor;
}

// AMBIENT LIGHT

struct AmbientLight {
  vec3 color;
};
uniform AmbientLight ambientLight;

vec3 illuminate(AmbientLight light) {
  return light.color;
}

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