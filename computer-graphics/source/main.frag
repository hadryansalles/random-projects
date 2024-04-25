#version 450 core

in vec3 fragColor;
in vec3 fragPos;

out vec4 outColor;

void main() {
    vec3 lightPos = vec3(0, 0, 0);
    float intensity = 2.0f;
    outColor = vec4(fragColor*intensity/length(lightPos - fragPos), 1.0);
}