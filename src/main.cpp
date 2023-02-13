#include "Assets.hpp"
#include "Lights.hpp"

#include <Workshop/Assets.hpp>
#include <Workshop/Camera.hpp>
#include <Workshop/Framebuffer.hpp>
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
#include <iostream>
#include <vector>

int main() {
  ws::Workshop workshop{800, 600, "Workshop App"};

  ws::Shader triangleShader{
      std::filesystem::path{ws::ASSETS_FOLDER / "shaders/triangle_without_vbo.vert"},
      std::filesystem::path{ws::ASSETS_FOLDER / "shaders/triangle_without_vbo.frag"}};
  ws::Shader solidColorShader{
      std::filesystem::path{ASSETS_FOLDER / "shaders/solid_color.vert"},
      std::filesystem::path{ASSETS_FOLDER / "shaders/solid_color.frag"}};
  ws::Shader diffuseShader{
      std::filesystem::path{ASSETS_FOLDER / "shaders/diffuse.vert"},
      std::filesystem::path{ASSETS_FOLDER / "shaders/diffuse.frag"}};
  ws::Framebuffer fbScene{800, 600};  // Render resolution. Can be smaller than window size.

  ws::Shader blurShader{
      std::filesystem::path{ASSETS_FOLDER / "shaders/fullscreen_quad_without_vbo.vert"},
      std::filesystem::path{ASSETS_FOLDER / "shaders/blur.frag"}};
  ws::Framebuffer fbBlur{800, 600};

  ws::Shader grayscaleShader{
      std::filesystem::path{ASSETS_FOLDER / "shaders/fullscreen_quad_without_vbo.vert"},
      std::filesystem::path{ASSETS_FOLDER / "shaders/fullscreen_quad_grayscale.frag"}};
  ws::Framebuffer fbGrayscale{800, 600};

  ws::Shader fullScreenShader{
      std::filesystem::path{ASSETS_FOLDER / "shaders/fullscreen_quad_without_vbo.vert"},
      std::filesystem::path{ASSETS_FOLDER / "shaders/fullscreen_quad_texture_sampler.frag"}};

  uint32_t vao;
  glGenVertexArrays(1, &vao);

  ws::DefaultMeshData meshData = ws::loadOBJ(ws::ASSETS_FOLDER / "models/suzanne.obj");
  ws::Mesh mesh{meshData};
  ws::Transform meshTransform{glm::vec3{0.1, 0.2, 0.3}, glm::vec3{0, 1, 0}, 0, glm::vec3{0.2, 0.2, 0.2}};

  ws::PerspectiveCamera3D cam;
  ws::AutoOrbitingCamera3DViewController orbitingCamController{cam};
  // cam.position = glm::vec3{0, 0, -5};
  // ws::ManualCamera3DViewController manualCamController{cam};

  std::vector<PointLight> lights;
  lights.push_back({.position = {0, 2, 0}, .color = {1, 1, 1}, .intensity = 1.0f});
  lights.push_back({.position = {2, 0, -2}, .color = {1, 0.5, 0.5}, .intensity = 1.0f});
  lights.push_back({.position = {-2, 0, 2}, .color = {0.5, 0.5, 1}, .intensity = 1.0f});

  while (!workshop.shouldStop()) {
    workshop.beginFrame();

    ImGui::Begin("Main");
    if (ImGui::Button("Reload shader")) {
      // triangleShader.reload();
      solidColorShader.reload();
      diffuseShader.reload();
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

    
    ImGui::SliderFloat("light[0].intensity", &lights[0].intensity, 0.f, 1.f);
    ImGui::SliderFloat("light[1].intensity", &lights[1].intensity, 0.f, 1.f);
    ImGui::SliderFloat("light[2].intensity", &lights[2].intensity, 0.f, 1.f);
    ImGui::Separator();

    static bool shouldShowImGuiDemo = false;
    ImGui::Checkbox("Show Demo", &shouldShowImGuiDemo);
    if (shouldShowImGuiDemo)
      ImGui::ShowDemoWindow();
    ImGui::End();

    static float t = 0.0f;
    meshTransform.rotation = glm::angleAxis(t, glm::vec3{0, 1, 0});
    t += 0.01f;

    {
      orbitingCamController.update(0.01f);

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
    glViewport(0, 0, 800, 600);

    diffuseShader.bind();
    mesh.bind();
    diffuseShader.setMatrix4fv("worldFromObject", glm::value_ptr(meshTransform.getWorldFromObjectMatrix()));
    diffuseShader.setMatrix4fv("viewFromWorld", glm::value_ptr(cam.getViewFromWorld()));
    diffuseShader.setMatrix4fv("projectionFromView", glm::value_ptr(cam.getProjectionFromView()));
    diffuseShader.setScalar1i("numPointLights", lights.size());
    for (size_t i = 0; i < lights.size(); ++i) {
      const PointLight& light = lights[i];
      diffuseShader.setVector3fv(fmt::format("pointLights[{}].color", i).c_str(), glm::value_ptr(light.color));
      diffuseShader.setVector3fv(fmt::format("pointLights[{}].position", i).c_str(), glm::value_ptr(light.position));
      diffuseShader.setScalar1f(fmt::format("pointLights[{}].intensity", i).c_str(), light.intensity);
    }
    mesh.draw();
    mesh.unbind();
    diffuseShader.unbind();
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
      glViewport(0, 0, 800, 600);
      blurShader.bind();
      blurShader.setScalar1f("u_Scale", blurScale);
      const float isHorizontalPass = static_cast<float>(i % 2 == 0);
      blurShader.setScalar1f("u_Horizontal", isHorizontalPass);
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
    glViewport(0, 0, 800, 600);
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
