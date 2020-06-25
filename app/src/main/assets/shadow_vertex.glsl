#version 300 es

uniform mat4 u_mvp;

layout(location = 0) in vec3 position;

void main() {
    gl_Position = u_mvp * vec4(position, 1.0f);
}
