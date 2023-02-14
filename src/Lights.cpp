#include "Lights.hpp"

#include <fmt/core.h>
#include <glm/gtc/type_ptr.hpp>

void PointLight::uploadToShader(const ws::Shader& shader, int ix) const {
  shader.setVector3fv(fmt::format("pointLights[{}].color", ix).c_str(), glm::value_ptr(color));
  shader.setVector3fv(fmt::format("pointLights[{}].position", ix).c_str(), glm::value_ptr(position));
  shader.setScalar1f(fmt::format("pointLights[{}].intensity", ix).c_str(), intensity);
}

void DirectionalLight::uploadToShader(const ws::Shader& shader, int ix) const {
  shader.setVector3fv(fmt::format("directionalLights[{}].color", ix).c_str(), glm::value_ptr(color));
  shader.setVector3fv(fmt::format("directionalLights[{}].direction", ix).c_str(), glm::value_ptr(direction));
  shader.setScalar1f(fmt::format("directionalLights[{}].intensity", ix).c_str(), intensity);
}