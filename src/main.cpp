#include "Assets.hpp"

#include <Workshop/Assets.hpp>
#include <Workshop/Framebuffer.hpp>
#include <Workshop/Model.hpp>
#include <Workshop/Shader.hpp>
#include <Workshop/Texture.hpp>
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

#include <iostream>

int main() {
  ws::Workshop workshop{800, 600, "Workshop App"};

  ws::Shader triangleShader{
      std::filesystem::path{ws::ASSETS_FOLDER / "shaders/triangle_without_vbo.vert"},
      std::filesystem::path{ws::ASSETS_FOLDER / "shaders/triangle_without_vbo.frag"}};
  ws::Shader solidColorShader{
      std::filesystem::path{ASSETS_FOLDER / "shaders/solid_color.vert"},
      std::filesystem::path{ASSETS_FOLDER / "shaders/solid_color.frag"}};
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

  ws::DefaultMeshData meshData = ws::loadOBJ(ws::ASSETS_FOLDER / "models/sphere_ico.obj");
  ws::Mesh mesh{meshData};

  while (!workshop.shouldStop()) {
    workshop.beginFrame();

    ImGui::Begin("Main");
    if (ImGui::Button("Reload shader")) {
      triangleShader.reload();
      blurShader.reload();
      grayscaleShader.reload();
      fullScreenShader.reload();
    }

    static glm::vec3 bgColor{42 / 256.0, 96 / 256.0, 87 / 256.0};
    ImGui::ColorEdit3("BG Color", glm::value_ptr(bgColor));

    static float blurScale = 1.0f;
    ImGui::SliderFloat("Blur Scale", &blurScale, 0.f, 10.f);

    static int numBlurPasses = 1;
    ImGui::SliderInt("# of Blur Passes", &numBlurPasses, 0, 10);

    static bool shouldShowImGuiDemo = false;
    ImGui::Checkbox("Show Demo", &shouldShowImGuiDemo);
    if (shouldShowImGuiDemo)
      ImGui::ShowDemoWindow();
    ImGui::End();

    // Scene Render Pass
    fbScene.bind();
    glClearColor(bgColor.x, bgColor.y, bgColor.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 800, 600);
    // triangleShader.bind();
    // glBindVertexArray(vao);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // glBindVertexArray(0);
    // triangleShader.unbind();

    solidColorShader.bind();
    mesh.bind();
    mesh.draw();
    mesh.unbind();
    solidColorShader.unbind();
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
      blurShader.SetScalar1f("u_Scale", blurScale);
      const float isHorizontalPass = static_cast<float>(i % 2 == 0);
      blurShader.SetScalar1f("u_Horizontal", isHorizontalPass);
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
