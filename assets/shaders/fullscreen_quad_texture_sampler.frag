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