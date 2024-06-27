#version 450 core

in vec3 fragPos;
in vec3 fragNormal;

uniform sampler2D albedoMap;
uniform sampler2D bumpMap;

uniform vec3 camPos;
uniform vec3 material;
uniform int mode;

uniform vec3 lightPos0;
uniform vec3 lightPos1;
uniform vec3 lightColor0;
uniform vec3 lightColor1;

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
    uv = uv * 0.5 + 0.5;
    return texture(sampler, uv).rgb;
}

void main() {
    vec3 albedo = sampleCubeMap(albedoMap, fragPos);
    vec3 bump = sampleCubeMap(bumpMap, fragPos);
    bump = normalize(bump * 2 - 1.0f);

    vec3 fragTangent = vec3(0, 1, 0);
    if (fragNormal.y == 1 || fragNormal.y == -1) {
        fragTangent = vec3(1, 0, 0);
    }

    vec3 fragNormal_ = normalize(fragNormal);
    vec3 fragBitangent = cross(fragTangent, fragNormal_);
    fragTangent = cross(fragNormal_, fragBitangent);

    vec3 ambient = vec3(0.1);
    vec3 diffuse = vec3(0, 0, 0);
    vec3 specular = vec3(0, 0, 0);

    vec3 V = normalize(camPos - fragPos);
    vec3 N = normalize(mat3(fragBitangent, fragTangent, fragNormal_) * bump);

    {
        vec3 L = lightPos0 - fragPos;
        vec3 L_ = normalize(L);
        vec3 R_ = reflect(-L_, N);
        diffuse += lightColor0 * material.x * max(dot(N, L_), 0) / dot(L, L);
        specular += lightColor0 * material.y * pow(max(dot(V, R_), 0), material.z) / dot(L, L);
    }
    {
        vec3 L = lightPos1 - fragPos;
        vec3 L_ = normalize(L);
        vec3 R_ = reflect(-L_, N);
        diffuse += lightColor1 * material.x * max(dot(N, L_), 0) / dot(L, L);
        specular += lightColor1 * material.y * pow(max(dot(V, R_), 0), material.z) / dot(L, L);
    }

    if (mode == 0) {
        if (material.x == -1) {
            if (length(lightPos0 - fragPos) < length(lightPos1 - fragPos)) {
                outColor = vec4(lightColor0, 1);
            } else {
                outColor = vec4(lightColor1, 1);
            }
        } else {
            outColor = vec4(albedo * (ambient + diffuse + specular), 1);
        }
    } else if (mode == 1) {
        outColor = vec4(albedo, 1);
    } else if (mode == 2) {
        outColor = vec4(abs(N), 1);
    }
}