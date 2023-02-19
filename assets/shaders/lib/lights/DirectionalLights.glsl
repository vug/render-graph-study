struct DirectionalLight {
  vec3 direction;
  vec3 color;
  float intensity;
};

#define MAX_DIRECTIONAL_LIGHTS 8
uniform DirectionalLight[MAX_DIRECTIONAL_LIGHTS] directionalLights;
uniform int numDirectionalLights;

vec3 illuminate(DirectionalLight light, vec3 normal) {
  return light.intensity * light.color * max(dot(-light.direction, normal), 0);
}