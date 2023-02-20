#include "Material.hpp"

#include <fmt/core.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <sstream>
#include <string>

Material::Material(const ws::Shader& shader)
    : shader(shader) {}

void Material::addParameter(std::string name, ParamT value) {
  assert(!parameters.contains(name));
  parameters.emplace(name, value);
}

void Material::setParameter(std::string name, ParamT value) {
  assert(parameters.contains(name));
  bool areOfSameType = parameters.at(name).index() == value.index();
  assert(areOfSameType);
  parameters.at(name) = value;
}

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

std::string Material::parametersToString() {
  std::stringstream ss;
  for (const auto& [name, value] : parameters) {
    std::visit(overloaded{
                   [&](int val) { ss << fmt::format("{} = {}\n", name, val); },
                   [&](float val) { ss << fmt::format("{} = {}\n", name, val); },
                   [&](const glm::vec2& val) { ss << fmt::format("{} = {}\n", name, glm::to_string(val)); },
                   [&](const glm::vec3& val) { ss << fmt::format("{} = {}\n", name, glm::to_string(val)); },
               },
               value);
  }
  return ss.str();
}