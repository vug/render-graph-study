#version 430

uniform vec4 u_Color = vec4(1.0, 1.0, 0.0, 1.0);

in VertexData {
  vec3 worldPosition;
  vec3 worldNormal;
} v;

layout (location = 0) out vec4 outColor;

const vec3 lightPosition = vec3(0, 3, 0);
const vec3 lightColor = vec3(1);
const float lightIntensity = 1.0;

void main() {
    vec3 wNormal = normalize(v.worldNormal);
    vec3 lightDir = normalize(v.worldPosition - lightPosition);
    vec3 diffuse = u_Color.rgb * lightIntensity * lightColor * max(dot(-lightDir, wNormal), 0);
    outColor = vec4(diffuse, 1);
}