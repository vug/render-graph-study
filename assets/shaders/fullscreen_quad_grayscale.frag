#version 300 es
#extension GL_EXT_separate_shader_objects : enable
precision mediump float;

layout (location = 0) in vec2 uv;

uniform sampler2D screenTexture;

layout (location = 0) out vec4 outColor;

void main () {
  vec3 tex =  texture(screenTexture, uv).rgb;
  float val = (tex.r + tex.g + tex.b) / 3.0;
  outColor.rgb = vec3(val);
}