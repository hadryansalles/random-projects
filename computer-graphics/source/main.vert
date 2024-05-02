#version 450 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

uniform mat4 model;

out vec3 fragColor;
out vec3 fragPos;
out vec3 fragNormal;

void main() {
    gl_Position = model * vec4(inPos, 1.0);
    fragNormal = (model * vec4(inNormal, 0.0)).xyz;
    fragPos = gl_Position.xyz;
    fragColor = vec3(0.3, 0.1, 0.5);
}