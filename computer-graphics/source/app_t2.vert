#version 450 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

uniform mat4 model;
uniform mat4 viewProjection;

out vec3 fragPos;
out vec3 fragNormal;

void main() {
    gl_Position = viewProjection * model * vec4(inPos, 1.0);
    fragPos = (model * vec4(inPos, 1.0)).xyz;
    fragNormal = inverse(transpose(mat3(model))) * inNormal;
}