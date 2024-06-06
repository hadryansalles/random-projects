#version 450 core

layout (location = 0) in vec3 inPos;

uniform vec3 color;

void main() {
    gl_Position = vec4(inPos.x, inPos.y, -0.5, 1.0);
}