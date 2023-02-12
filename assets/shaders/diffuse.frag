#version 430

in VertexData {
  vec3 worldPosition;
  vec3 worldNormal;
} v;

uniform vec4 u_Color = vec4(1.0, 1.0, 0.0, 1.0);
uniform int numPointLights;
struct PointLight {
  vec3 position;
  vec3 color;
  float intensity;
};
uniform PointLight[3] pointLights;

layout (location = 0) out vec4 outColor;

const vec3 lightPosition = vec3(0, 3, 0);
const vec3 lightColor = vec3(1);
const float lightIntensity = 1.0;

void main() {
    vec3 wNormal = normalize(v.worldNormal);
    vec3 pointDiffuse = vec3(0);
    for (int i = 0; i < numPointLights; i++) {
      PointLight light = pointLights[i];
      vec3 lightDir = normalize(v.worldPosition - light.position);
      pointDiffuse += light.intensity * light.color * max(dot(-lightDir, wNormal), 0);
    }
    vec3 diffuse = u_Color.rgb * pointDiffuse;
    outColor = vec4(diffuse, 1);
}