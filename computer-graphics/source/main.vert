#version 450 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

uniform mat4 model;
uniform mat4 viewProjection;
uniform vec3 color;

out vec3 fragPos;
out vec3 fragColor;
out vec2 fragUV;

void main() {
    gl_Position = viewProjection * model * vec4(inPos, 1.0);
    fragPos = gl_Position.xyz;
    fragColor = vec3(inUV, 0.0);
    fragUV = inUV;
}