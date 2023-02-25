#pragma once

#include "Object.hpp"

#include <Workshop/Camera.hpp>
#include <Workshop/Lights.hpp>
#include <Workshop/Model.hpp>
#include <Workshop/Shader.hpp>

#include <vector>

namespace ws {

class Scene {
 public:
  std::vector<Object> objects;
  //
  std::vector<DirectionalLight> directionalLights;
  std::vector<PointLight> pointLights;
  HemisphericalLight hemisphericalLight;
  AmbientLight ambientLight;

  void draw(const ICamera& cam) const;
  void drawObject(const Object& obj, const ICamera& cam) const;
  void uploadAllLightsToShader(const Shader& shader) const;
  void uploadSceneUniformsToShader(const Shader& shader, const ICamera& cam) const;
};

}  // namespace ws