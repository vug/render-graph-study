#version 430

in VertexData {
  vec2 uv;
} v;

uniform sampler2D screenTexture;

layout (location = 0) out vec4 outColor;

void main () { 
  // outColor = vec4(uv.x, uv.y, 0, 1.0); 
  outColor.rgb = texture(screenTexture, v.uv).rgb;
}