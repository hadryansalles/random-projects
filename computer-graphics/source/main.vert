#version 450 core

layout (location = 0) in vec3 inPos;

uniform mat4 model;

out vec3 fragColor;

void main() {
    gl_Position = model * vec4(inPos, 1.0);
    fragColor = (gl_Position.xyz + vec3(1.0)) / 2;
}