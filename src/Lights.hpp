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