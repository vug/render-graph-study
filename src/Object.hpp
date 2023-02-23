#pragma once

#include <Workshop/Material.hpp>
#include <Workshop/Model.hpp>
#include <Workshop/Transform.hpp>

#include <string>

struct Object {
  std::string name;
  ws::Transform transform;
  ws::Mesh mesh;
  ws::Material material;
};