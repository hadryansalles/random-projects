/*
 * @author Hadryan Salles
 */

#ifndef CLGM_H
#define CLGM_H

#include <string.h>
#include <math.h>
#include <stdlib.h>

#define K_PI 3.14159265359f

inline float radians(float degrees) {
    return K_PI * degrees / 180.0f;
}

typedef struct {
    float x, y, z;
} xyz_type;

typedef struct {
    union {
        xyz_type;
        float data[3];
    };
} vec3;

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

inline vec3 vec3_new(float x, float y, float z) {
    vec3 v = {
        .x = x,
        .y = y,
        .z = z,
    };
    return v;
}

inline vec3 vec3_min(vec3 a, vec3 b) {
    return vec3_new(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z));
}

inline vec3 vec3_max(vec3 a, vec3 b) {
    return vec3_new(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z));
}

inline vec3 vec3_from_float(float a) {
    return vec3_new(a, a, a);
}

inline vec4 vec4_new(float x, float y, float z, float w) {
    vec4 v = {
        .x = x,
        .y = y,
        .z = z,
        .w = w,
    };
    return v;
}

inline vec3 vec3_zero() {
    return vec3_new(0, 0, 0);
}

inline vec4 vec4_zero() {
    return vec4_new(0, 0, 0, 0);
}

inline vec3 vec3_rand() { 
    return vec3_new(rand()/(float)RAND_MAX, rand()/(float)RAND_MAX, rand()/(float)RAND_MAX); 
}

inline vec3 vec3_add(vec3 a, vec3 b) {
    return vec3_new(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline vec3 vec3_sub(vec3 a, vec3 b) {
    return vec3_new(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline vec3 vec3_mul(vec3 a, float b) {
    return vec3_new(a.x * b, a.y * b, a.z * b);
}

inline vec3 vec3_cross(vec3 a, vec3 b) {
    vec3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

inline float vec3_dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3 vec3_normalize(vec3 a) {
    return vec3_mul(a, 1.0f/sqrtf(vec3_dot(a, a)));
}

inline float vec3_length(vec3 a) {
    return sqrt(vec3_dot(a, a));
}

inline vec4 vec4_sub(vec4 a, vec4 b) {
    return vec4_new(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline vec4 vec4_mul(vec4 a, float b) {
    return vec4_new(a.x * b, a.y * b, a.z * b, a.w * b);
}

inline mat4 mat4_identity() {
    mat4 m = {
        .cols = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1},
        }
    };
    return m;
}

inline mat4 mat4_mul(mat4 a, mat4 b) {
    mat4 c;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float acc = 0;
            for (int k = 0; k < 4; k++) {
                acc += a.data[i * 4 + k] * b.data[k* 4 + j];
            }
            c.data[i*4 + j] = acc;
        }
    }
    return c;
}

inline mat4 mat4_transpose(mat4 a) {
    mat4 b;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            b.data[4 * i + j] = a.data[4 * j + i];
        }
    }
    return b;
}

inline vec4 mat4_apply(mat4 a, vec4 v) {
    float* m = a.data;
    vec4 r = {
        .x = v.x * m[0] + v.y * m[4] + v.z * m[8] + v.w * m[12],
        .y = v.x * m[1] + v.y * m[5] + v.z * m[9] + v.w * m[13],
        .z = v.x * m[2] + v.y * m[6] + v.z * m[10] + v.w * m[14],
        .w = v.x * m[3] + v.y * m[7] + v.z * m[11] + v.w * m[15],
    };
    return r;
}

inline vec4 quat_mul(vec4 a, vec4 b) {
    vec4 c;
    c.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    c.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    c.y = a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z;
    c.z = a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x;
    return c;
}

inline vec4 quat_from_axis_angle(vec3 axis, float degrees) {
    float halfRad = radians(degrees) / 2.0f;
    float s = sinf(halfRad) / vec3_length(axis);
    vec4 q;
    q.x = axis.x * s;
    q.y = axis.y * s;
    q.z = axis.z * s;
    q.w = cosf(halfRad);
    return q;
}

inline vec4 quat_from_euler(vec3 degrees) {
    vec4 qx = quat_from_axis_angle(vec3_new(1, 0, 0), degrees.x);
    vec4 qy = quat_from_axis_angle(vec3_new(0, 1, 0), degrees.y);
    vec4 qz = quat_from_axis_angle(vec3_new(0, 0, 1), degrees.z);
    return quat_mul(qx, quat_mul(qy, qz));
}

inline vec4 quat_identity() {
    return vec4_new(0, 0, 0, 1);
}

inline vec4 quat_inverse(vec4 q) {
    return vec4_new(-q.x, -q.y, -q.z, q.w);
}

inline mat4 mat4_from_quat(vec4 q) {
    mat4 m = {
        .cols = {
            {1 - 2*q.y*q.y - 2*q.z*q.z, 2*q.x*q.y + 2*q.w*q.z, 2*q.x*q.z - 2*q.w*q.y, 0},
            {2*q.x*q.y - 2*q.w*q.z, 1 - 2*q.x*q.x - 2*q.z*q.z, 2*q.y*q.z + 2*q.w*q.x, 0},
            {2*q.x*q.z + 2*q.w*q.y, 2*q.y*q.z - 2*q.w*q.x, 1 - 2*q.x*q.x - 2*q.y*q.y, 0},
            {0, 0, 0, 1},
        }
    };
    return mat4_transpose(m);
}

inline vec3 vec3_from_vec4(vec4 v) {
    return vec3_new(v.x, v.y, v.z);
}

inline void vec3_print(vec3 v) {
    printf("%.3f %.3f %.3f\n", v.x, v.y, v.z);
}

inline void vec4_print(vec4 v) {
    printf("%.3f %.3f %.3f %.3f\n", v.x, v.y, v.z, v.w);
}

inline mat4 mat4_print(mat4 m) {
    printf("-------\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%.3f ", m.data[4*j + i]);
        }
        printf("\n");
    }
    printf("-------\n");
}

inline vec4 vec4_from_vec3(vec3 v, float w) {
    return vec4_new(v.x, v.y, v.z, w);
}

inline mat4 mat4_transform(vec3 scale, vec4 quat, vec3 translation) {
    mat4 m = mat4_identity();
    m.cols[0].x *= scale.x;
    m.cols[1].y *= scale.y;
    m.cols[2].z *= scale.z;
    m = mat4_mul(mat4_from_quat(quat), m);
    m.cols[3] = vec4_from_vec3(translation, 1.0f);
    //m.cols[3] = vec4_new(0, 0, 0, 1);
    return m;
}

inline mat4 mat4_scale(vec3 scale) {
    mat4 m = mat4_identity();
    m.cols[0].x *= scale.x;
    m.cols[1].y *= scale.y;
    m.cols[2].z *= scale.z;
    return m;
}

inline mat4 mat4_transform_inverse(vec3 scale, vec4 quat, vec3 translation) {
    mat4 m = mat4_identity();
    m.cols[0].x *= scale.x;
    m.cols[1].y *= scale.y;
    m.cols[2].z *= scale.z;
    m = mat4_mul(mat4_from_quat(quat_inverse(quat)), m);
    m.cols[3] = vec4_sub(vec4_new(0, 0, 0, 1), vec4_from_vec3(translation, 0.0f));
    return m;

    //mat4 m = mat4_from_quat(quat_inverse(quat));
    //m.cols[0].x /= scale.x;
    //m.cols[1].y /= scale.y;
    //m.cols[2].z /= scale.z;
    //m.cols[3] = vec4_sub(vec4_new(0, 0, 0, 1), vec4_from_vec3(translation, 0.0f));
    //return m;
}

inline mat4 mat4_translation(vec3 translation) {
   mat4 m = mat4_identity();
   m.cols[3] = vec4_from_vec3(translation, 1.0f); 
   return m;
}

inline mat4 mat4_oblique(float alpha, float beta) {
    mat4 m = mat4_identity();
    m.cols[2].x = 0.5f * cosf(radians(alpha));
    m.cols[2].y = 0.5f * cosf(radians(beta));
    m.cols[2].z = 0.0f;
    return m;
}

inline mat4 mat4_ortho(float l, float r, float b, float t, float n, float f) {
    mat4 m = mat4_identity();
    m.cols[0].x = 2.0f / (r - l);
    m.cols[1].y = 2.0f / (t - b);
    m.cols[2].z = 2.0f / (n - f);
    m.cols[3].x = -(r + l) / (r - l);
    m.cols[3].y = -(t + b) / (t - b);
    m.cols[3].z = -(n + f) / (n - f);
    m.cols[3].w = 1.0f;
    return m;
}

inline mat4 mat4_perspective(float fovy, float aspect, float near, float far) {
    float tanHalfFovy = tan(radians(fovy)/2.0f);
    mat4 m = mat4_identity();
    m.cols[0].x = 1.0f / (aspect * tanHalfFovy);
    m.cols[1].y = 1.0f / tanHalfFovy;
    m.cols[2].z = -(far + near)/(far - near);
    m.cols[2].w = -1.0f;
    m.cols[3].z = -1.0*far*near/(far - near);
    m.cols[3].w = 0.0f; 
    return m;
}

inline mat4 mat4_lookat(vec3 eye, vec3 target, vec3 up) {
    vec3 forward = vec3_normalize(vec3_sub(target, eye));
    vec3 right = vec3_normalize(vec3_cross(forward, up));
    up = vec3_normalize(vec3_cross(right, forward));
    mat4 m = mat4_identity();
    m.cols[0] = vec4_new(right.x, up.x, -forward.x, 0);
    m.cols[1] = vec4_new(right.y, up.y, -forward.y, 0);
    m.cols[2] = vec4_new(right.z, up.z, -forward.z, 0);
    m.cols[3] = vec4_new(-vec3_dot(right, eye), -vec3_dot(up, eye), vec3_dot(forward, eye), 1);
    return m;
}

#endif