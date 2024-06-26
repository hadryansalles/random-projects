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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include <tinyobj_loader_c.h>

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

typedef struct {
    char* buffers[1024];
    int nextBuffer;
} LoaderCtx;

void get_file_data(void* ctx, const char* filename, const int is_mtl, const char* obj_filename, char** data, size_t* len) {
    LoaderCtx* loaderCtx = ctx;
    loaderCtx->buffers[loaderCtx->nextBuffer] = file_read(filename, len);
    *data = loaderCtx->buffers[loaderCtx->nextBuffer++];
}

Mesh mesh_read_parser(const char* filename) {
    Mesh mesh;
    mesh.ibo = 0;
    tinyobj_attrib_t attrib;
    tinyobj_shape_t* shapes;
    tinyobj_material_t* materials;
    size_t numShapes = 0;
    size_t numMaterials = 0;
    LoaderCtx ctx;
    ctx.nextBuffer = 0;
    unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
    int ret =  tinyobj_parse_obj(&attrib, &shapes, &numShapes, &materials, &numMaterials, filename, get_file_data, &ctx, flags);

    if (ret == TINYOBJ_SUCCESS) {
        printf("num_faces=%d num_vertices=%d num_uvs=%d\n", attrib.num_faces, attrib.num_vertices, attrib.num_texcoords);
        vec3* vertices = malloc(attrib.num_vertices * sizeof(vec3));
        vec3* normals = malloc(attrib.num_vertices * sizeof(vec3));
        uint32_t* indices = malloc(3 * attrib.num_faces * sizeof(uint32_t));
        int vertexCount = attrib.num_vertices;
        for (int i = 0; i < attrib.num_vertices; i++) {
            vertices[i].x = attrib.vertices[3 * i + 0];
            vertices[i].y = attrib.vertices[3 * i + 1];
            vertices[i].z = attrib.vertices[3 * i + 2];
        }
        for (int i = 0; i < attrib.num_faces; i++) {
            indices[i] = attrib.faces[i].v_idx;
        }
        mesh_gen_normals(vertices, normals, indices, vertexCount, attrib.num_faces / 3);
        mesh_normalize(vertices, vertexCount);
        mesh = mesh_create(vertices, normals, indices, vertexCount, attrib.num_faces / 3);
        free(indices);
        free(vertices);
        free(normals);
    } else {
        printf("Error %d in tinyobj_parse_obj\n", ret);
    }

    for (int i = 0; i < ctx.nextBuffer; i++) {
        free(ctx.buffers[i]);
    }
    tinyobj_materials_free(materials, numMaterials);
    tinyobj_shapes_free(shapes, numShapes);
    tinyobj_attrib_free(&attrib);
    return mesh;
}

void mesh_normalize(vec3* vertices, int vertexCount) {
    vec3 vmin = vec3_from_float(1000000.0);
    vec3 vmax = vec3_from_float(-1000000.0);
    for (int i = 0; i < vertexCount; i++) {
        vmin = vec3_min(vmin, vertices[i]);
        vmax = vec3_max(vmax, vertices[i]);
    }

    vec3 diff = vec3_sub(vmax, vmin);
    float ratio = 1.0f / (fmax(diff.x, fmax(diff.y, diff.z)));

    for (int i = 0; i < vertexCount; i++) {
        vertices[i] = vec3_mul(vec3_sub(vertices[i], vmin), ratio);
        vertices[i] = vec3_mul(vec3_sub(vertices[i], vec3_from_float(0.5)), 1.5);
    }
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

    // read data
    for (int i = 0; i < filesize; i++) {
        if (filebuffer[i] == 'v' && filebuffer[i+1] == 'n') {
            vec3* v = &(normals[vni]);
            int count = sscanf(&filebuffer[i], "vn %f %f %f", &(v->x), &(v->y), &(v->z));
            if (count == 3) {
                vni++;
                continue;
            }
            printf("Invalid normal...\n");
        } else if (filebuffer[i] == 'v' && filebuffer[i+1] == 't') {
            // read uv
        } else if (filebuffer[i] == 'v' && (i == 0 || filebuffer[i - 1] == '\n') && (filebuffer[i + 1] == ' ')) {
            vec3* v = &(vertices[vi]);
            int count = sscanf(&filebuffer[i], "v %f %f %f", &(v->x), &(v->y), &(v->z));
            if (count == 3) {
                vi++;
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

    mesh_normalize(vertices, vertexCount);

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

void mesh_gen_normals(const vec3* vertices, vec3* normals, const unsigned int* indices, int vertexCount, int triangleCount) {
    for (int i = 0; i < vertexCount; i++) {
        normals[i] = vec3_zero();
    }

    for (int i = 0; i < triangleCount * 3; i += 3) {
        vec3 v0 = vertices[indices[i + 0]];
        vec3 v1 = vertices[indices[i + 1]];
        vec3 v2 = vertices[indices[i + 2]];
        vec3 e0 = vec3_sub(v1, v0);
        vec3 e1 = vec3_sub(v2, v0);
        vec3 n = vec3_normalize(vec3_cross(e0, e1));
        normals[indices[i + 0]] = vec3_add(normals[indices[i + 0]], n);
        normals[indices[i + 1]] = vec3_add(normals[indices[i + 1]], n);
        normals[indices[i + 2]] = vec3_add(normals[indices[i + 2]], n);
    }

    for (int i = 0; i < vertexCount; i++) {
        normals[i] = vec3_normalize(normals[i]);
    }
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

Mesh mesh_create_normals_uvs(const vec3* vertices, const vec3* normals, const vec2* uvs, const unsigned int* indices, int vertexCount, int triangleCount) { Mesh mesh;
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

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vboUvs);
    glBufferData(GL_ARRAY_BUFFER, 1 * vertexCount * sizeof(vec2), uvs, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 1 * sizeof(vec2), (void*)0);
    glEnableVertexAttribArray(2);

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

Mesh mesh_create_vertices(const vec3* vertices, int vertexCount) {
    Mesh mesh;
    mesh.vertexCount = vertexCount;
    mesh.triangleCount = 0;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vboVertices);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vboVertices);
    glBufferData(GL_ARRAY_BUFFER, 1 * vertexCount * sizeof(vec3), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); 

    return mesh;
}

void mesh_update_vertices(Mesh* mesh, const vec3* vertices, int vertexCount) {
    mesh->vertexCount = vertexCount;

    glBindVertexArray(mesh->vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vboVertices);
    glBufferData(GL_ARRAY_BUFFER, 1 * vertexCount * sizeof(vec3), vertices, GL_DYNAMIC_DRAW);

    glBindVertexArray(0); 
}

void mesh_draw_lines(Mesh mesh) {
    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_LINE_STRIP, 0, mesh.vertexCount);
    glBindVertexArray(0);
}

void mesh_draw_polygon(Mesh mesh) {
    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, mesh.vertexCount);
    glBindVertexArray(0);
}

void mesh_draw_points(Mesh mesh, int first, int last) {
    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_POINTS, first, last);
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

unsigned int texture_load(const char* filename) {
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t channels = 0;
    stbi_set_flip_vertically_on_load(1);
    uint8_t* data = stbi_load(filename, &width, &height, &channels, 4);
    unsigned int texture = 0;
    if (data != NULL) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("width=%d height=%d channels=%d\n", width, height, channels);
    } else {
        printf("Error loading texture '%s'\n", filename);
    }
    stbi_image_free(data);
    return texture;
}