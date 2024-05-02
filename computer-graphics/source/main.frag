#version 450 core

in vec3 fragColor;
in vec3 fragPos;
in vec3 fragNormal;

out vec4 outColor;

void main() {
    vec3 lightPos = vec3(0, 0.5, -2.5);
    float intensity = 6.0f;
    vec3 ambientLight = vec3(0.2, 0.4, 0.5);
    vec3 lightDir = fragPos - lightPos;
    float lambert = dot(normalize(lightDir), normalize(fragNormal));
    outColor = vec4(pow(ambientLight*2.0*(1.0 - lambert) + fragColor*intensity*lambert/length(lightDir), vec3(2.2)), 1.0);
}