#include "Material.hpp"

#include <glm/gtc/type_ptr.hpp>

Material::Material(const ws::Shader& shader)
    : shader(shader) {}

void Material::uploadUniform(const std::string& name, const ParamT& value) {
  std::visit(overloaded{
                 [&](int val) { shader.setScalar1i(name.c_str(), val); },
                 [&](float val) { shader.setScalar1f(name.c_str(), val); },
                 [&](const glm::vec2& val) { shader.setVector2fv(name.c_str(), glm::value_ptr(val)); },
                 [&](const glm::vec3& val) { shader.setVector3fv(name.c_str(), glm::value_ptr(val)); },
             },
             value);
}

void Material::uploadParameters() {
  for (const auto& [name, value] : parameters)
    uploadUniform(name, value);
}