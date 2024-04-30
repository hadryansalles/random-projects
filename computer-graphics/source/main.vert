#version 450 core

layout (location = 0) in vec3 inPos;

uniform mat4 model;

out vec3 fragColor;
out vec3 fragPos;

void main() {
    gl_Position = model * vec4(inPos, 1.0);
    fragPos = gl_Position.xyz;
    fragColor = vec3(0.5, 0.1, 0.9);
}