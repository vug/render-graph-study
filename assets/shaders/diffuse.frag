#version 430

in VertexData {
  vec3 worldPosition;
  vec3 worldNormal;
} v;

uniform vec4 u_Color = vec4(1.0, 1.0, 0.0, 1.0);

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

layout (location = 0) out vec4 outColor;

void main() {
    vec3 wNormal = normalize(v.worldNormal);
    vec3 diffuseLight = vec3(0);
    for (int i = 0; i < numPointLights; i++)
      diffuseLight += illuminate(pointLights[i], v.worldPosition, wNormal);
    for (int i = 0; i < numPointLights; i++)
      diffuseLight += illuminate(directionalLights[i], wNormal);
    vec3 diffuse = u_Color.rgb * diffuseLight;
    outColor = vec4(diffuse, 1);
}