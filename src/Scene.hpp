#pragma once

#include "Object.hpp"

#include <Workshop/Lights.hpp>

#include <vector>

struct Scene {
  std::vector<Object> objects;
  std::vector<ws::DirectionalLight> directionalLights;
  std::vector<ws::PointLight> pointLights;
  ws::HemisphericalLight hemisphericalLight;
  ws::AmbientLight ambientLight;
};