/*
 * @author Hadryan Salles
 */

#include "core.h"
#include "cglm.h"

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

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
    glShaderSource(vertexShader, 1, (const char* const*)&vertexShaderSource, NULL);
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
    glShaderSource(fragmentShader, 1, (const char* const*)&fragmentShaderSource, NULL);
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

Mesh mesh_read(const char* filename) {
    unsigned int filesize = 0;
    char* filebuffer = file_read(filename, &filesize);

    printf("Finished reading mesh file.\n");

    unsigned int vertexCount = 0;
    unsigned int normalCount = 0;
    unsigned int indexCount = 0;

    // count number of elements
    for (int i = 0; i < filesize; i++) {
        if (filebuffer[i] == 'v' && filebuffer[i+1] == 'n') {
            normalCount++;
        } else if (filebuffer[i] == 'v' && filebuffer[i + 1] == 't') {
            // tex count ++
        } else if (filebuffer[i] == 'v' && (i == 0 || filebuffer[i - 1] == '\n') && (filebuffer[i + 1] == ' ')) {
            vertexCount++;
        } else if (filebuffer[i] == 'f' && filebuffer[i-1] == '\n' && filebuffer[i+1] == ' ') {
            indexCount++;
        }
    }

    printf("Finished counting vertiices.\n");

    vec3* vertices = (vec3*)malloc(vertexCount * sizeof(vec3));
    vec3* genNormals = (vec3*)malloc(vertexCount * sizeof(vec3));
    vec3* normals = (vec3*)malloc(normalCount * sizeof(vec3));
    unsigned int* indices = (unsigned int*)malloc(indexCount * sizeof(unsigned int) * 3);
    unsigned int* normalIndices = (unsigned int*)malloc(indexCount * sizeof(unsigned int) * 3);
    unsigned int* uvIndices = (unsigned int*)malloc(indexCount * sizeof(unsigned int) * 3);

    int vi = 0;
    int vni = 0;
    int fi = 0;
    int fni = 0;
    int fui = 0;

    vec3 vmin = vec3_from_float(1000000.0);
    vec3 vmax = vec3_from_float(-1000000.0);

    // read data
    for (int i = 0; i < filesize; i++) {
        if (filebuffer[i] == 'v' && filebuffer[i+1] == 'n') {
            // vec3* v = &(normals[vni]);
            // int count = sscanf(&filebuffer[i], "vn %f %f %f", &(v->x), &(v->y), &(v->z));
            // if (count == 3) {
            //     vni++;
            //     continue;
            // }
            // printf("Invalid normal...\n");
        } else if (filebuffer[i] == 'v' && filebuffer[i+1] == 't') {
            // read uv
        } else if (filebuffer[i] == 'v' && (i == 0 || filebuffer[i - 1] == '\n') && (filebuffer[i + 1] == ' ')) {
            vec3* v = &(vertices[vi]);
            int count = sscanf(&filebuffer[i], "v %f %f %f", &(v->x), &(v->y), &(v->z));
            if (count == 3) {
                vi++;
                vmin = vec3_min(vmin, *v);
                vmax = vec3_max(vmax, *v);
                continue;
            }
            printf("Invalid vertex...\n");
        } else if (filebuffer[i] == 'f') {
            unsigned int* v = &(indices[fi]);
            unsigned int* n = &(normalIndices[fni]);
            unsigned int* u = &(uvIndices[fui]);
            int count = sscanf(&filebuffer[i], "f %d %d %d", &(v[0]), &(v[1]), &(v[2]));
            if (count == 3) {
                fi += 3;
                continue;
            }
            count = sscanf(&filebuffer[i], "f %d/%d/%d %d/%d/%d %d/%d/%d", &(v[0]), &(u[0]), &(n[0]), &(v[1]), &(u[1]), &(n[1]), &(v[2]), &(u[2]), &(n[2]));
            if (count == 9) {
                fi += 3;
                fni += 3;
                fui += 3;
                continue;
            }
            count = sscanf(&filebuffer[i], "f %d//%d %d//%d %d//%d", &(v[0]), &(n[0]), &(v[1]), &(n[1]), &(v[2]), &(n[2]));
            if (count == 6) {
                fi += 3;
                fni += 3;
                continue;
            }
        }
    }

    printf("Finished parsing vertices.\n");

    vec3 diff = vec3_sub(vmax, vmin);
    float ratio = 1.0f / (fmax(diff.x, fmax(diff.y, diff.z)));

    for (int i = 0; i < vertexCount; i++) {
        vertices[i] = vec3_mul(vec3_sub(vertices[i], vmin), ratio);
        vertices[i] = vec3_mul(vec3_sub(vertices[i], vec3_from_float(0.5)), 1.5);
    }

    for (int i = 0; i < fi; i++) {
        indices[i]--;
    }


    for (int i = 0; i < vertexCount; i++) {
        genNormals[i] = vec3_zero();
    }

    for (int i = 0; i < indexCount * 3; i += 3) {
        vec3 v0 = vertices[indices[i + 0]];
        vec3 v1 = vertices[indices[i + 1]];
        vec3 v2 = vertices[indices[i + 2]];
        vec3 e0 = vec3_sub(v1, v0);
        vec3 e1 = vec3_sub(v2, v0);
        vec3 n = vec3_normalize(vec3_cross(e1, e0));
        genNormals[indices[i + 0]] = vec3_add(genNormals[indices[i + 0]], n);
        genNormals[indices[i + 1]] = vec3_add(genNormals[indices[i + 1]], n);
        genNormals[indices[i + 2]] = vec3_add(genNormals[indices[i + 2]], n);
    }

    for (int i = 0; i < vertexCount; i++) {
        genNormals[i] = vec3_normalize(genNormals[i]);
    }

    Mesh mesh = mesh_create(vertices, genNormals, indices, vertexCount, indexCount);

    free(filebuffer);
    free(vertices);
    free(genNormals);
    free(normals);
    free(indices);
    free(uvIndices);
    free(normalIndices);

    return mesh;
}

Mesh mesh_create_non_normals(const vec3* vertices, const unsigned int* indices, int vertexCount, int triangleCount) {
    Mesh mesh;
    mesh.vertexCount = vertexCount;
    mesh.triangleCount = triangleCount;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vboVertices);
    glGenBuffers(1, &mesh.ibo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vboVertices);
    glBufferData(GL_ARRAY_BUFFER, 1 * vertexCount * sizeof(vec3), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleCount * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glBindVertexArray(0); 

    return mesh;
}

Mesh mesh_create(const vec3* vertices, const vec3* normals, const unsigned int* indices, int vertexCount, int triangleCount) {
    Mesh mesh;
    mesh.vertexCount = vertexCount;
    mesh.triangleCount = triangleCount;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vboVertices);
    glGenBuffers(1, &mesh.vboNormals);
    glGenBuffers(1, &mesh.ibo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vboVertices);
    glBufferData(GL_ARRAY_BUFFER, 1 * vertexCount * sizeof(vec3), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vboNormals);
    glBufferData(GL_ARRAY_BUFFER, 1 * vertexCount * sizeof(vec3), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(vec3), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleCount * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glBindVertexArray(0); 

    return mesh;
}

void mesh_draw_lines(Mesh mesh) {
    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_LINE_STRIP, 0, mesh.vertexCount);
    glBindVertexArray(0);
}

void mesh_draw(Mesh mesh) {
    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, mesh.triangleCount*3, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

void mesh_delete(Mesh mesh) {
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.vboVertices);
    glDeleteBuffers(1, &mesh.vboNormals);
    glDeleteBuffers(1, &mesh.ibo);
}

mat4 object_get_model(Object* object) {
    return mat4_transform(object->size, quat_from_euler(object->rotation), object->position);
}

mat4 camera_get_view_projection(Camera* camera, float aspect) {
    mat4 projection = mat4_perspective(camera->fov, aspect, 0.001f, 1000.0f);
    mat4 camTranslation = mat4_translation(vec3_sub(vec3_zero(), camera->position));
    mat4 camRotationMat = mat4_from_quat(quat_inverse(camera->rotation));
    mat4 view = mat4_mul(camTranslation, camRotationMat);
    return mat4_mul(view, projection);
}