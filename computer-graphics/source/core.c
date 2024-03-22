#include "core.h"

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gl/gl.h>
#include <math.h>

vec3 vec3_add(vec3 a, vec3 b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

vec3 vec3_sub(vec3 a, vec3 b) {
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
}

vec3 vec3_mul(vec3 a, float b) {
    a.x *= b;
    a.y *= b;
    a.z *= b;
    return a;
}

vec3 vec3_cross(vec3 a, vec3 b) {
    vec3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

float vec3_dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vec3_length(vec3 a) {
    return sqrt(vec3_dot(a, a));
}

char* file_read(const char* filename, unsigned int* outsize) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        printf("Failed to read file %s\n", filename);
        return 0;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (size == 0) {
        printf("Empty file %s\n", filename);
        return 0;
    }
    char* buffer = malloc(size + 1);
    fread(buffer, size, 1, f);
    fclose(f);
    buffer[size] = 0;
    *outsize = size;
    return buffer;
}

unsigned int shader_create(const char* vertexShaderFile, const char* fragmentShaderFile) {
    unsigned int vertexSize = 0;
    unsigned int fragmentSize = 0;
    char* vertexShaderSource = file_read(vertexShaderFile, &vertexSize);
    char* fragmentShaderSource = file_read(fragmentShaderFile, &fragmentSize);
    if (vertexSize == 0 || fragmentSize == 0) {
        return 0;
    }
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
        return 0;
    }
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
        return 0;
    }
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
        return 0;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexShaderSource);
    free(fragmentShaderSource);

    return shaderProgram;
}

Mesh mesh_create(const vec3* vertices, int count) {
    Mesh mesh;
    mesh.count = count;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * count * sizeof(vec3), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)sizeof(vec3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    return mesh;
}

void mesh_update(Mesh mesh, const vec3* vertices, int count) {
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * count * sizeof(vec3), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void mesh_draw(Mesh mesh) {
    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_LINE_STRIP, 0, mesh.count);
}

void mesh_delete(Mesh mesh) {
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.vbo);
}