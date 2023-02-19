struct PointLight {
  vec3 position;
  vec3 color;
  float intensity;
};

#define MAX_POINT_LIGHTS 8
uniform PointLight[MAX_POINT_LIGHTS] pointLights;
uniform int numPointLights;

vec3 illuminate(PointLight light, vec3 position, vec3 normal) {
  vec3 r = position - light.position;
  float r2 = dot(r, r);
  vec3 lightDir = normalize(r);
  return light.intensity * light.color * max(dot(-lightDir, normal), 0) / r2;
}