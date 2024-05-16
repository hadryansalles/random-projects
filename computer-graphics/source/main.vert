#version 450 core

layout (location = 0) in vec3 inPos;

uniform mat4 model;
uniform mat4 viewProjection;
uniform vec3 color;

out vec3 fragPos;
out vec3 fragColor;

void main() {
    gl_Position = viewProjection * model * vec4(inPos, 1.0);
    fragPos = gl_Position.xyz;
    fragColor = color;
}