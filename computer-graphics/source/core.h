/*
 * @author Hadryan Salles
 */

#ifndef CORE_H
#define CORE_H

#include "cglm.h"

char* file_read(const char* filename, unsigned int* size);
unsigned int shader_create(const char* vertexFile, const char* fragmentFile);

typedef struct {
    unsigned int vao;
    unsigned int vboVertices;
    unsigned int vboNormals;
    unsigned int vboUvs;
    unsigned int ibo;
    unsigned int vertexCount;
    unsigned int triangleCount;
} Mesh;

void mesh_normalize(vec3* vertices, int vertexCount);
Mesh mesh_read(const char* filename);
Mesh mesh_read_parser(const char* filename);
Mesh mesh_create_non_normals(const vec3* vertices, const unsigned int* indices, int vertexCount, int triangleCount);
Mesh mesh_create(const vec3* vertices, const vec3* normals, const unsigned int* indices, int vertexCount, int triangleCount);
Mesh mesh_create_normals_uvs(const vec3* vertices, const vec3* normals, const vec2* uvs, const unsigned int* indices, int vertexCount, int triangleCount);
void mesh_draw(Mesh mesh);
void mesh_delete(Mesh mesh);

int texture_load(const char* filename);

typedef struct {
    Mesh mesh;
    vec3 position;
    vec3 size;
    vec3 rotation;
    unsigned int texture;
} Object;

typedef struct {
    vec3 position;
    float fov;
    vec4 rotation;
} Camera;

mat4 object_get_model(Object* object);
mat4 camera_get_view_projection(Camera* camera, float aspect);

#endif