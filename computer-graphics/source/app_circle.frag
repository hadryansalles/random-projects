#version 450 core

uniform ivec3 position;
uniform vec3 color;

uniform ivec3 center;
uniform float radius;

in vec3 fragPos;
out vec4 outColor;

void main() {
    ivec2 diff = ivec2(gl_FragCoord.xy) - position.z * position.xy;
    if (diff.x >= 0 && diff.x < position.z && diff.y >= 0 && diff.y < position.z) {
        outColor = vec4(fragPos.x + 1.0, color.x + color.y / 2.0, fragPos.y + 1.0, 1);
    } else {
        outColor = vec4(0, 0, 0, 0);
    }
}