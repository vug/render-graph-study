#include "Scene.hpp"

namespace ws {
void Scene::draw(const ICamera& cam) const {
  for (const Object& obj : objects)
    drawObject(obj, cam);
}

void Scene::drawObject(const Object& obj, const ICamera& cam) const {
  const ws::Mesh& mesh = obj.mesh;
  const ws::Material& material = obj.material;
  const ws::Shader& shader = material.shader;
  shader.bind();
  mesh.bind();
  // shader.setScalar1f("specularCoeff", specularCoeff);
  uploadSceneUniformsToShader(shader, cam);
  shader.setMatrix4("worldFromObject", obj.transform.getWorldFromObjectMatrix());
  uploadAllLightsToShader(shader);
  material.uploadParameters();
  mesh.draw();
  mesh.unbind();
  shader.unbind();
}

void Scene::uploadAllLightsToShader(const Shader& shader) const {
  for (size_t i = 0; i < pointLights.size(); ++i)
    pointLights[i].uploadToShader(shader, i);
  for (size_t i = 0; i < directionalLights.size(); ++i)
    directionalLights[i].uploadToShader(shader, i);
  hemisphericalLight.uploadToShader(shader);
  ambientLight.uploadToShader(shader);
}

void Scene::uploadSceneUniformsToShader(const Shader& shader, const ICamera& cam) const {
  shader.setMatrix4("viewFromWorld", cam.getViewFromWorld());
  shader.setMatrix4("projectionFromView", cam.getProjectionFromView());
  shader.setInteger("numPointLights", pointLights.size());
  shader.setInteger("numDirectionalLights", directionalLights.size());
  shader.setVector3("eyePos", cam.getPosition());
}
}  // namespace ws