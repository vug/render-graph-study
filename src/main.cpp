#include "Assets.h"

#include <Workshop/Assets.h>
#include <Workshop/Framebuffer.h>
#include <Workshop/Shader.h>
#include <Workshop/Texture.h>
#include <Workshop/Workshop.h>

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

#include <iostream>

int main() {
  ws::Workshop workshop{800, 600, "Workshop App"};

  ws::Shader triangleShader{
      std::filesystem::path{ws::ASSETS_FOLDER / "shaders/triangle_without_vbo.vert"},
      std::filesystem::path{ws::ASSETS_FOLDER / "shaders/triangle_without_vbo.frag"}};
  ws::Framebuffer fbScene{800, 600};  // Render resolution. Can be smaller than window size.

  ws::Shader grayscaleShader{
      std::filesystem::path{ASSETS_FOLDER / "shaders/fullscreen_quad_without_vbo.vert"},
      std::filesystem::path{ASSETS_FOLDER / "shaders/fullscreen_quad_grayscale.frag"}};
  ws::Framebuffer fbGrayscale{800, 600};

  ws::Shader fullScreenShader{
      std::filesystem::path{ASSETS_FOLDER / "shaders/fullscreen_quad_without_vbo.vert"},
      std::filesystem::path{ASSETS_FOLDER / "shaders/fullscreen_quad_texture_sampler.frag"}};

  uint32_t vao;
  glGenVertexArrays(1, &vao);

  while (!workshop.shouldStop()) {
    workshop.beginFrame();

    ImGui::Begin("Main");
    if (ImGui::Button("Reload shader")) {
      triangleShader.reload();
      grayscaleShader.reload();
      fullScreenShader.reload();
    }
    static bool shouldShowImGuiDemo = false;
    ImGui::Checkbox("Show Demo", &shouldShowImGuiDemo);
    if (shouldShowImGuiDemo)
      ImGui::ShowDemoWindow();

    static glm::vec3 bgColor{42 / 256.0, 96 / 256.0, 87 / 256.0};
    ImGui::ColorEdit3("BG Color", glm::value_ptr(bgColor));
    ImGui::End();

    // Scene Render Pass
    fbScene.bind();
    glClearColor(bgColor.x, bgColor.y, bgColor.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 800, 600);
    triangleShader.bind();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    triangleShader.unbind();
    fbScene.unbind();

    // Grayscale Pass Post-Process
    fbGrayscale.bind();
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 800, 600);
    grayscaleShader.bind();
    fbScene.getColorAttachment().bind();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    fbScene.getColorAttachment().unbind();
    fbGrayscale.unbind();

    // Display final Framebuffer on screen
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 800, 600);
    fullScreenShader.bind();
    fbGrayscale.getColorAttachment().bind();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    fbGrayscale.getColorAttachment().unbind();
    fullScreenShader.unbind();

    workshop.endFrame();
  }

  return 0;
}
