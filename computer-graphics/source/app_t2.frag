#version 450 core

in vec3 fragPos;
in vec3 fragNormal;

uniform sampler2D texture1;
uniform vec3 camPos;
uniform vec3 lightPos;
uniform float Kd;
uniform float Ks;

out vec4 outColor;

vec3 sampleCubeMap(sampler2D sampler, vec3 n) {
    vec3 nAbs = abs(n);
    float m = max(nAbs.x, max(nAbs.y, nAbs.z));
    vec2 uv = vec2(0.0, 0.0);
    if (m == n.x) {
        uv = vec2(n.y, n.z);
    } else if (m == -n.x) {
        uv = vec2(n.y, -n.z);
    } else if (m == n.y) {
        uv = vec2(n.x, n.z);
    } else if (m == -n.y) {
        uv = vec2(n.x, -n.z);
    } else if (m == n.z) {
        uv = vec2(n.x, n.y);
    } else {
        uv = vec2(-n.x, n.y);
    }
    uv *= 1.0/m;
    return texture(sampler, uv).rgb;
}

void main() {
    vec3 lightColor = 20 * vec3(1, 1, 1);

    vec3 N = normalize(fragNormal);
    vec3 V = normalize(camPos - fragPos);
    vec3 L = lightPos - fragPos;
    vec3 L_ = normalize(L);
    vec3 R_ = reflect(-L_, N);

    vec3 albedo = sampleCubeMap(texture1, N);
    // vec3 albedo = vec3(0.3, 0.2, 0.8);
    float ambient = 0.3;
    float diffuse = Kd * max(dot(N, L_), 0);
    float specular = Ks * pow(max(dot(V, R_), 0), 128);

    // outColor = vec4(N, 1);
    outColor = vec4(albedo * (ambient + (diffuse + specular) * lightColor / dot(L, L)), 1);
}