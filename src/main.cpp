#include "Assets.hpp"
#include "Object.hpp"
#include "Scene.hpp"

#include <Workshop/Assets.hpp>
#include <Workshop/Camera.hpp>
#include <Workshop/Framebuffer.hpp>
#include <Workshop/Lights.hpp>
#include <Workshop/Material.hpp>
#include <Workshop/Model.hpp>
#include <Workshop/Shader.hpp>
#include <Workshop/Texture.hpp>
#include <Workshop/Transform.hpp>
#include <Workshop/Workshop.hpp>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

// #include <implot.h>
// #include <stb_image.h>
// #include <tiny_obj_loader.h>
// #include <vivid/vivid.h>

#include <fmt/core.h>
#include <vector>

struct RenderGraph {
  std::vector<ws::Framebuffer> framebuffers;
};

int main() {
  ws::Workshop workshop{800, 600, "Workshop App"};
  glm::uvec2 winSize{workshop.getWindowSize()};
  RenderGraph renderGraph;

  ws::Shader::makeNamedStringsFromFolder(ASSETS_FOLDER / "shaders/lib");

  ws::Shader triangleShader{
      std::filesystem::path{ws::ASSETS_FOLDER / "shaders/triangle_without_vbo.vert"},
      std::filesystem::path{ws::ASSETS_FOLDER / "shaders/triangle_without_vbo.frag"}};
  ws::Shader solidColorShader{
      std::filesystem::path{ASSETS_FOLDER / "shaders/solid_color.vert"},
      std::filesystem::path{ASSETS_FOLDER / "shaders/solid_color.frag"}};
  ws::Shader phongShader{
      std::filesystem::path{ASSETS_FOLDER / "shaders/phong.vert"},
      std::filesystem::path{ASSETS_FOLDER / "shaders/phong.frag"}};
  renderGraph.framebuffers.reserve(16);  // so that when the vector resizes returned references continue to work
  // Render resolution. Can be smaller than window size.
  ws::Framebuffer& fbScene = renderGraph.framebuffers.emplace_back(winSize.x, winSize.y);

  ws::Shader blurShader{
      std::filesystem::path{ASSETS_FOLDER / "shaders/fullscreen_quad_without_vbo.vert"},
      std::filesystem::path{ASSETS_FOLDER / "shaders/blur.frag"}};
  ws::Framebuffer& fbBlur = renderGraph.framebuffers.emplace_back(winSize.x, winSize.y);

  // ws::Shader grayscaleShader{
  //     std::filesystem::path{ASSETS_FOLDER / "shaders/fullscreen_quad_without_vbo.vert"},
  //     std::filesystem::path{ASSETS_FOLDER / "shaders/fullscreen_quad_grayscale.frag"}};
  // ws::Framebuffer& fbGrayscale = renderGraph.framebuffers.emplace_back(winSize.x, winSize.y);

  ws::Shader fullScreenShader{
      std::filesystem::path{ASSETS_FOLDER / "shaders/fullscreen_quad_without_vbo.vert"},
      std::filesystem::path{ASSETS_FOLDER / "shaders/fullscreen_quad_texture_sampler.frag"}};

  uint32_t vao;
  glGenVertexArrays(1, &vao);

  Scene scene;

  Object monkey {
    .transform = {glm::vec3{0.1, 0.2, 0.3}, glm::vec3{0, 1, 0}, 0, glm::vec3{0.2, 0.2, 0.2}},
    .mesh = {ws::loadOBJ(ws::ASSETS_FOLDER / "models/suzanne_smooth.obj")},
    .material = {phongShader},
  };
  monkey.material.addParameter("specularCoeff", 2.0f); // .addParameter<float>("specularCoeff");

  ws::PerspectiveCamera3D cam;
  ws::AutoOrbitingCamera3DViewController orbitingCamController{cam};
  // cam.position = glm::vec3{0, 0, -5};
  // ws::ManualCamera3DViewController manualCamController{cam};

  scene.directionalLights.push_back({.direction = {0, -2, 0}, .color = {1, 1, 1}, .intensity = 0.2f});
  scene.pointLights.push_back({.position = {2, 0, -2}, .color = {1, 0.5, 0.5}, .intensity = 2.0f});
  scene.pointLights.push_back({.position = {-2, 0, 2}, .color = {0.5, 0.5, 1}, .intensity = 2.0f});
  scene.hemisphericalLight = {.northColor = {0.2, 0.2, 0.9}, .southColor = {0.9, 0.2, 0.2}, .intensity = 0.1f};
  scene.ambientLight = {.color = {0, 0, 0}};

  while (!workshop.shouldStop()) {
    workshop.beginFrame();
    winSize = workshop.getWindowSize();
    for (auto& fb : renderGraph.framebuffers)
      fb.resizeIfNeeded(winSize.x, winSize.y);

    ImGui::Begin("Main");
    if (ImGui::Button("Reload shader")) {
      // triangleShader.reload();
      solidColorShader.reload();
      monkey.material.shader.reload();
      blurShader.reload();
      // grayscaleShader.reload();
      // fullScreenShader.reload();
    }

    static glm::vec3 bgColor{42 / 256.0, 96 / 256.0, 87 / 256.0};
    ImGui::ColorEdit3("BG Color", glm::value_ptr(bgColor));

    static float blurScale = 1.0f;
    ImGui::SliderFloat("Blur Scale", &blurScale, 0.f, 10.f);

    static int numBlurPasses = 1;
    ImGui::SliderInt("# of Blur Passes", &numBlurPasses, 0, 10);

    ImGui::SliderFloat("directionalLight[0].intensity", &scene.directionalLights[0].intensity, 0.f, 3.f);
    ImGui::SliderFloat("pointLight[0].intensity", &scene.pointLights[0].intensity, 0.f, 3.f);
    ImGui::SliderFloat("pointLight[1].intensity", &scene.pointLights[1].intensity, 0.f, 3.f);
    ImGui::SliderFloat("hemisphericalLight.intensity", &scene.hemisphericalLight.intensity, 0.f, 3.f);
    ImGui::SliderFloat("ambientLight.green", &scene.ambientLight.color.y, 0.f, 1.f);
    static float specularCoeff = 32.0f;
    ImGui::SliderFloat("specularCoeff", &specularCoeff, 0.1f, 128.f);
    monkey.material.setParameter("specularCoeff", specularCoeff);
    std::string paramsStr = monkey.material.parametersToString();
    ImGui::Separator();
    ImGui::Text(paramsStr.c_str());

    static bool shouldShowImGuiDemo = false;
    ImGui::Checkbox("Show Demo", &shouldShowImGuiDemo);
    if (shouldShowImGuiDemo)
      ImGui::ShowDemoWindow();
    ImGui::End();

    static float t = 0.0f;
    monkey.transform.rotation = glm::angleAxis(t, glm::vec3{0, 1, 0});
    t += 0.01f;

    {
      orbitingCamController.update(0.01f);
      cam.aspectRatio = static_cast<float>(winSize.x) / winSize.y;

      // double cx, cy;
      // glfwGetCursorPos(workshop.getGLFWwindow(), &cx, &cy);
      // glm::vec2 cursorPos{cx, cy};
      // int glfwMouseInput = (glfwGetMouseButton(workshop.getGLFWwindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) ? 0 : ((glfwGetMouseButton(workshop.getGLFWwindow(), 1) == GLFW_PRESS) ? GLFW_MOUSE_BUTTON_RIGHT : -1);
      // manualCamController.update(cursorPos, glfwMouseInput);
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Scene Render Pass
    fbScene.bind();
    glClearColor(bgColor.x, bgColor.y, bgColor.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, winSize.x, winSize.y);

    monkey.material.shader.bind();
    monkey.mesh.bind();
    monkey.material.shader.setMatrix4("worldFromObject", monkey.transform.getWorldFromObjectMatrix());
    monkey.material.shader.setMatrix4("viewFromWorld", cam.getViewFromWorld());
    monkey.material.shader.setMatrix4("projectionFromView", cam.getProjectionFromView());
    monkey.material.shader.setInteger("numPointLights", scene.pointLights.size());
    monkey.material.shader.setInteger("numDirectionalLights", scene.directionalLights.size());
    monkey.material.shader.setVector3("eyePos", cam.getPosition());
    // monkey.material.shader.setScalar1f("specularCoeff", specularCoeff);
    monkey.material.uploadParameters();
    for (size_t i = 0; i < scene.pointLights.size(); ++i)
      scene.pointLights[i].uploadToShader(monkey.material.shader, i);
    for (size_t i = 0; i < scene.directionalLights.size(); ++i)
      scene.directionalLights[i].uploadToShader(monkey.material.shader, i);
    scene.hemisphericalLight.uploadToShader(monkey.material.shader);
    scene.ambientLight.uploadToShader(monkey.material.shader);
    monkey.mesh.draw();
    monkey.mesh.unbind();
    monkey.material.shader.unbind();
    fbScene.unbind();

    // Blur Pass Post-Process
    const int n = 2 * numBlurPasses;
    for (int i = 0; i < n; ++i) {
      const bool isFirst = i == 0;
      ws::Framebuffer& fbInput = isFirst ? fbScene : fbBlur;
      fbBlur.bind();
      if (isFirst) {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      }
      glViewport(0, 0, winSize.x, winSize.y);
      blurShader.bind();
      blurShader.setFloat("u_Scale", blurScale);
      const float isHorizontalPass = static_cast<float>(i % 2 == 0);
      blurShader.setFloat("u_Horizontal", isHorizontalPass);
      fbInput.getColorAttachment().bind();
      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
      fbInput.getColorAttachment().unbind();
      blurShader.unbind();
      fbBlur.unbind();
    }

    // // Grayscale Pass Post-Process
    // fbGrayscale.bind();
    // glClearColor(1, 0, 1, 1);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glViewport(0, 0, 800, 600);
    // grayscaleShader.bind();
    // fbScene.getColorAttachment().bind();
    // glBindVertexArray(vao);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    // glBindVertexArray(0);
    // fbScene.getColorAttachment().unbind();
    // grayscaleShader.unbind();
    // fbGrayscale.unbind();

    ws::Framebuffer& fbScreen = fbBlur;

    // Display final Framebuffer on screen
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, winSize.x, winSize.y);
    fullScreenShader.bind();
    fbScreen.getColorAttachment().bind();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    fbScreen.getColorAttachment().unbind();
    fullScreenShader.unbind();

    workshop.endFrame();
  }

  return 0;
}
