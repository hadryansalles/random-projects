#version 450 core

in vec3 fragPos;
in vec3 fragColor;
in vec2 fragUV;

uniform sampler2D texture1;

out vec4 outColor;

void main() {
    // vec3 light = 3 * vec3(0.5, 0.3, 0.7) / dot(fragPos, fragPos);
    // outColor = vec4(fragColor * light, 1);
    outColor = vec4(texture(texture1, fragUV).rgb, 1);
}