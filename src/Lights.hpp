#include <Workshop/Shader.hpp>

#include <glm/vec3.hpp>

class PointLight {
 public:
  glm::vec3 position;
  glm::vec3 color;
  float intensity;

  void uploadToShader(const ws::Shader& shader, int ix) const;
};

class DirectionalLight {
 public:
  glm::vec3 direction;
  glm::vec3 color;
  float intensity;

  void uploadToShader(const ws::Shader& shader, int ix) const;
};

class HemisphericalLight {
 public:
  glm::vec3 northColor;
  glm::vec3 southColor;
  float intensity;

  void uploadToShader(const ws::Shader& shader) const;
};

class AmbientLight {
 public:
  glm::vec3 color;

  void uploadToShader(const ws::Shader& shader) const;
};