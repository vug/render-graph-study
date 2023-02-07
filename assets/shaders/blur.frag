// https://github.com/skaarj1989/FrameGraph-Example/blob/master/shaders/Blur.frag
#version 430
// #extension GL_GOOGLE_include_directive : require

// https://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/

in VertexData {
  vec2 uv;
} v;

layout(binding = 0) uniform sampler2D t_0;

#define getTexelSize(src) (1.0 / textureSize(src, 0))
#define calculateMipLevels(src) floor(log2(float(textureSize(src, 0).x))) + 1.0

layout(location = 0) uniform bool u_Horizontal;
layout(location = 1) uniform float u_Scale = 1.0;

const float kOffsets[3] = {0.0, 1.3846153846, 3.2307692308};
const float kWeights[3] = {0.2270270270, 0.3162162162, 0.0702702703};

vec3 horizontalBlur() {
  const float texOffset = getTexelSize(t_0).x * u_Scale;
  vec3 result = texture(t_0, v.uv).rgb * kWeights[0];
  for (uint i = 1; i < 3; ++i) {
    result +=
      texture(t_0, v.uv + vec2(texOffset * i, 0.0)).rgb * kWeights[i];
    result +=
      texture(t_0, v.uv - vec2(texOffset * i, 0.0)).rgb * kWeights[i];
  }
  return result;
}
vec3 verticalBlur() {
  const float texOffset = getTexelSize(t_0).y * u_Scale;
  vec3 result = texture(t_0, v.uv).rgb * kWeights[0];
  for (uint i = 1; i < 3; ++i) {
    result +=
      texture(t_0, v.uv + vec2(0.0, texOffset * i)).rgb * kWeights[i];
    result +=
      texture(t_0, v.uv - vec2(0.0, texOffset * i)).rgb * kWeights[i];
  }
  return result;
}

layout(location = 0) out vec3 FragColor;
void main() { FragColor = u_Horizontal ? horizontalBlur() : verticalBlur(); }