#version 430

in VertexData {
  vec2 uv;
} v;

uniform sampler2D screenTexture;

layout (location = 0) out vec4 outColor;

void main () {
  vec3 tex =  texture(screenTexture, v.uv).rgb;
  float val = (tex.r + tex.g + tex.b) / 3.0;
  outColor.rgb = vec3(val);
}