#version 450 core

layout (location = 0) in vec3 inPos;
// layout (location = 1) in vec3 inColor;

uniform mat4 model;

out vec3 fragColor;

void main() {
    fragColor = inPos + vec3(0.5, 0.5, 0.5);
    gl_Position = model * vec4(inPos.xyz, 1.0);
}