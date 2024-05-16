#version 450 core

in vec3 fragPos;
in vec3 fragColor;

out vec4 outColor;

void main() {
    vec3 light = 40 * vec3(0.5, 0.3, 0.7) / dot(fragPos, fragPos);
    outColor = vec4(fragColor * light, 1);
}