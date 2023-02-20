#pragma once

#include <Workshop/Shader.hpp>

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <variant>

using ParamT = std::variant<int, float, glm::vec2, glm::vec3>;

template <typename T>
// concept ParamC = std::is_same_v<int, T> || std::is_same_v<float, T> || std::is_same_v<glm::vec2, T> || std::is_convertible_v<T, ParamT>;
concept ParamC = std::is_convertible_v<T, ParamT>;

template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

class Material {
 private:
  const ws::Shader& shader;
  std::unordered_map<std::string, ParamT> parameters;

  void uploadUniform(const std::string& name, const ParamT& value);

 public:
  Material(const ws::Shader& shader);

  template <ParamC P>
  void addParameter(std::string name) {
    parameters.insert({name, P{}});
  }

  template <ParamC P>
  void setParameter(std::string name, P value) {
    parameters[name] = value;
  }

  void uploadParameters();
};