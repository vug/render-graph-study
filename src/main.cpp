#include <Shader.h>
#include <Texture.h>
#include <Framebuffer.h>
#include <Workshop.h>

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

  const char *fullScreenVertexShader = R"(
#version 300 es
#extension GL_EXT_separate_shader_objects : enable
precision mediump float;

layout (location = 0) out vec2 fragUV;

vec2 positions[4] = vec2[](vec2(-1, -1), vec2(1, -1), vec2(1, 1), vec2(-1, 1));
vec2 uvs[4] = vec2[](vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(0, 1));
int indices[6] = int[](0, 1, 2, 0, 2, 3);

void main ()
{
  int ix = indices[gl_VertexID];
	gl_Position = vec4 (positions[ix], 0.0, 1.0);
	fragUV = uvs[ix];
}
  )";

  const char *fullScreenFragmentShader = R"(
#version 300 es
#extension GL_EXT_separate_shader_objects : enable
precision mediump float;

layout (location = 0) in vec2 uv;

uniform sampler2D screenTexture;

layout (location = 0) out vec4 outColor;

void main () { 
  // outColor = vec4(uv.x, uv.y, 0, 1.0); 
  outColor.rgb = texture(screenTexture, uv).rgb;
}
  )";

  ws::Shader fullScreenShader{fullScreenVertexShader, fullScreenFragmentShader};
  ws::Shader triangleShader = ws::Shader{
      std::filesystem::path{"assets/workshop/shaders/triangle_without_vbo_vert.glsl"},
      std::filesystem::path{"assets/workshop/shaders/triangle_without_vbo_frag.glsl"}};
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
