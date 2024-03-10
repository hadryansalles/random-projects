#ifndef CORE_H
#define CORE_H

char* file_read(const char* filename, unsigned int* size);
unsigned int shader_create(const char* vertexFile, const char* fragmentFile);

typedef struct {
    float x, y, z;
} xyz_type;

typedef struct {
    union {
        xyz_type;
        float data[3];
    };
} vec3;

vec3 vec3_add(vec3 a, vec3 b);

typedef struct {
    unsigned int vao;
    unsigned int vbo;
    unsigned int count;
} Mesh;

Mesh mesh_create(const vec3* vertices, int count);
Mesh mesh_create_colors(const float* vertices, const float* color, int count);
void mesh_update(Mesh mesh, const vec3* vertices, int count);
void mesh_draw(Mesh mesh);
void mesh_delete(Mesh mesh);

#endif