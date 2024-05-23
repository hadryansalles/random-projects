/*
 * @author Hadryan Salles
 */

#ifndef CLGM_H
#define CLGM_H

#define K_PI 3.14159265359f

typedef struct xy_type {
    float x, y;
} xy_type;

typedef struct xyz_type {
    float x, y, z;
} xyz_type;

typedef struct vec3 {
    union {
        xyz_type;
        float data[3];
    };
} vec3;

typedef struct vec2 {
    union {
        xy_type;
        float data[2];
    };
} vec2;

typedef struct {
    float x, y, z, w;
} xyzw_type;

typedef struct {
    union {
        xyzw_type;
        float data[4];
    };
} vec4;

typedef struct {
    union {
        float data[9];
        vec3 cols[3];
    };
} mat3;

typedef struct {
    union {
        float data[16];
        vec4 cols[4];
    };
} mat4;

float radians(float degrees);
vec3 vec3_new(float x, float y, float z);
vec3 vec3_min(vec3 a, vec3 b);
vec3 vec3_max(vec3 a, vec3 b);
vec3 vec3_from_float(float a);
vec4 vec4_new(float x, float y, float z, float w);
vec3 vec3_zero();
vec4 vec4_zero();
vec3 vec3_rand();
vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_mul(vec3 a, float b);
vec3 vec3_cross(vec3 a, vec3 b);
float vec3_dot(vec3 a, vec3 b);
vec3 vec3_normalize(vec3 a);
float vec3_length(vec3 a); 
vec4 vec4_sub(vec4 a, vec4 b);
vec4 vec4_mul(vec4 a, float b);
mat4 mat4_identity();
mat4 mat4_mul(mat4 a, mat4 b);
mat4 mat4_transpose(mat4 a);
vec3 mat4_apply(mat4 a, vec3 v);
vec4 mat4_apply_vec4(mat4 a, vec4 v);
vec4 quat_mul(vec4 a, vec4 b);
vec4 quat_from_axis_angle(vec3 axis, float degrees);
vec4 quat_from_euler_zxy(vec3 degrees);
vec4 quat_from_euler(vec3 degrees);
vec4 quat_identity();
vec4 quat_inverse(vec4 q);
mat4 mat4_from_quat(vec4 q);
vec3 vec3_from_vec4(vec4 v);
void vec3_print(vec3 v);
void vec4_print(vec4 v);
void mat4_print(mat4 m);
vec4 vec4_from_vec3(vec3 v, float w);
mat4 mat4_transform(vec3 scale, vec4 quat, vec3 translation);
mat4 mat4_scale(vec3 scale);
mat4 mat4_transform_inverse(vec3 scale, vec4 quat, vec3 translation);
mat4 mat4_translation(vec3 translation);
mat4 mat4_oblique(float alpha, float beta);
mat4 mat4_ortho(float l, float r, float b, float t, float n, float f);
mat4 mat4_perspective(float fovy, float aspect, float near, float far);
mat4 mat4_lookat(vec3 eye, vec3 target, vec3 up);

#endif