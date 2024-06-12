#version 450 core

layout (location = 0) in vec3 inPos;

out vec3 fragPos;

void main() {
    fragPos = inPos;
    gl_Position = vec4(inPos.x, inPos.y, 0, 1.0);
}