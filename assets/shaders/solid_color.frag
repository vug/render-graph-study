#version 430

uniform vec4 u_Color = vec4(1.0, 1.0, 0.0, 1.0);

layout (location = 0) out vec4 outColor;

void main() {
    outColor = u_Color;
}