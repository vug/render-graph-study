#version 430

// uniform mat4 u_ModelViewPerspective;

layout(location = 0) in vec3 a_Position;

void main() {
    // gl_Position = u_ModelViewPerspective * vec4(a_Position, 1.0);
    gl_Position = vec4(a_Position, 1);
}
