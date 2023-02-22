#pragma once

#include <Workshop/Material.hpp>
#include <Workshop/Model.hpp>
#include <Workshop/Transform.hpp>

struct Object {
  ws::Transform transform;
  ws::Mesh mesh;
  ws::Material material;
};