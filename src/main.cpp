#include <Workshop/Assets.h>
#include <Workshop/Shader.h>
#include <Workshop/Texture.h>
#include <Workshop/Framebuffer.h>
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

int main()
{
  ws::Workshop workshop{800, 600, "Workshop App"};

  ws::Shader fullScreenShader{
      std::filesystem::path{"assets/shaders/fullscreen_quad_without_vbo.vert"},
      std::filesystem::path{"assets/shaders/fullscreen_quad_texture_sampler.frag"}};
  ws::Shader triangleShader{
      std::filesystem::path{ws::ASSETS_FOLDER / "shaders/triangle_without_vbo_vert.glsl"},
      std::filesystem::path{ws::ASSETS_FOLDER / "shaders/triangle_without_vbo_frag.glsl"}};
  ws::Framebuffer fbScene{800, 600}; // Render resolution. Can be smaller than window size.

  while (!workshop.shouldStop())
  {
    workshop.beginFrame();

    ImGui::Begin("Main");
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
    glDrawArrays(GL_TRIANGLES, 0, 3);
    triangleShader.unbind();
    fbScene.unbind();

    // Display final Framebuffer on screen
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 800, 600);
    fullScreenShader.bind();
    fbScene.getColorAttachment().bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    fbScene.getColorAttachment().unbind();
    fullScreenShader.unbind();

    workshop.endFrame();
  }

  return 0;
}
