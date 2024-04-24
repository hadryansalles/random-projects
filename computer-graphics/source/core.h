/*
 * @author Hadryan Salles
 */

#ifndef CORE_H
#define CORE_H

char* file_read(const char* filename, unsigned int* size);
unsigned int shader_create(const char* vertexFile, const char* fragmentFile);

typedef struct {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ibo;
    unsigned int vertexCount;
    unsigned int triangleCount;
} Mesh;

Mesh mesh_read(const char* filename);
Mesh mesh_create(const struct vec3* vertices, const unsigned int* indices, int vertexCount, int triangleCount);
void mesh_draw(Mesh mesh);
void mesh_delete(Mesh mesh);

#endif