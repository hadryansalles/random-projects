#ifndef CORE_H
#define CORE_H

#define K_PI 3.14159265359f

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
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_mul(vec3 a, float b);
vec3 vec3_cross(vec3 a, vec3 b);
float vec3_dot(vec3 a, vec3 b);
float vec3_length(vec3 a);

char* file_read(const char* filename, unsigned int* size);
unsigned int shader_create(const char* vertexFile, const char* fragmentFile);

typedef struct {
    unsigned int vao;
    unsigned int vbo;
    unsigned int count;
} Mesh;

Mesh mesh_create(const vec3* vertices, int count);
void mesh_update(Mesh mesh, const vec3* vertices, int count);
void mesh_draw(Mesh mesh);
void mesh_delete(Mesh mesh);

#endif