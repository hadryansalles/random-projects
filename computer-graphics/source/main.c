/*
 * @author Hadryan Salles
 */

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gl/gl.h>
#include <math.h>
#include <time.h>

#include "core.h"
#include "cglm.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void questions();

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 1280;

int main() {
    questions();

    srand(time(NULL));

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSV40 Hadryan Salles", NULL, NULL);
    if (window == NULL) {
        printf("Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1); // vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    unsigned int shaderProgram = shader_create("source/main.vert", "source/main.frag");

    vec3 vertices[] = {
        {-1, -1,  1},
        {-1,  1,  1},
        { 1,  1,  1},
        { 1, -1,  1},

        {-1, -1, -1},
        {-1,  1, -1},
        { 1,  1, -1},
        { 1, -1, -1},
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,

        7, 6, 5,
        5, 4, 7,

        4, 5, 1,
        1, 0, 4,

        3, 2, 6,
        6, 7, 3,

        1, 5, 6,
        6, 2, 1,

        4, 0, 3,
        3, 7, 4
    };

    Mesh mesh = mesh_create(vertices, indices, 8, 12);

    float rotSpeed = 1.6f;
    float scaleSpeed = 1.02f;
    vec3 translation = vec3_zero();
    vec3 scale = vec3_new(0.5, 0.5, 0.5);
    vec3 rotation = vec3_zero();
    vec3 rotationDir = vec3_rand();
    int modelLocation = glGetUniformLocation(shaderProgram, "model");
    clock_t startClock = clock();

    glEnable(GL_DEPTH_TEST);  
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
        glUseProgram(shaderProgram);

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            rotation.x += rotSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            rotation.x -= rotSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            rotation.y += rotSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            rotation.y -= rotSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            rotation.z -= rotSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            rotation.z += rotSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            rotation = vec3_zero();
            translation = vec3_zero();
            scale = vec3_new(0.5, 0.5, 0.5);
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            scale = vec3_mul(scale, 1.0f/scaleSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            scale = vec3_mul(scale, scaleSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            translation.y += 0.01;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            translation.y -= 0.01;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            translation.x += 0.01;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            translation.x -= 0.01;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
            rotation = vec3_add(rotation, vec3_mul(rotationDir, rotSpeed));
            float ms = (double)(clock() - startClock) / CLOCKS_PER_SEC;
            float s = 0.2f + 0.2f * (1.0f + cosf(1.5*ms))/2.0f;
            scale = vec3_new(s, s, s);
        }

        mat4 model = mat4_transform(scale, quat_from_euler(rotation), translation);
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.data);

        mesh_draw(mesh);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    mesh_delete(mesh);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void questions() {
    {
        vec4 q = quat_from_euler(vec3_new(90, 0, 0));
        mat4 rx = mat4_from_quat(q);
        q = quat_from_euler(vec3_new(0, 0, 90));
        mat4 rz = mat4_from_quat(q);
        printf("rx ");
        mat4_print(rx);
        printf("rz ");
        mat4_print(rz);
        printf("rx*rz ");
        mat4_print(mat4_mul(rx, rz));
        printf("rz*rx ");
        mat4_print(mat4_mul(rz, rx));
    }
    {
        vec3 scale = vec3_new(2, 4, 1 / 3.0f);
        vec4 rotation = quat_identity();
        vec3 translation = vec3_new(1, -2, 7);
        mat4 t = mat4_transform(scale, rotation, translation);
        mat4 invt = mat4_transform_inverse(scale, rotation, translation);
        vec4 p0 = mat4_apply(t, vec4_new(0, 0, 0, 1));
        vec4 p1 = mat4_apply(t, vec4_new(1, 1, 1, 1));
        printf("transform ");
        mat4_print(t);
        printf("transformed P0: ");
        vec4_print(p0);
        printf("transformed P1: ");
        vec4_print(p1);
        printf("inverse transform ");
        mat4_print(invt);
    }
    {
        printf("R((1,1,1), 90): ");
        mat4_print(mat4_from_quat(quat_from_axis_angle(vec3_new(1,1,1), 90)));
    }
    {
        vec3 point = vec3_new(3, 0, 6);
        mat4 rotation = mat4_from_quat(quat_from_euler(vec3_new(0, 30, 0)));
        mat4 toOrigin = mat4_translation(vec3_sub(vec3_zero(), point));
        mat4 toPoint = mat4_translation(point);
        printf("Rotation: ");
        mat4_print(rotation);
        printf("ToOrigin: ");
        mat4_print(toOrigin);
        printf("ToPoint: ");
        mat4_print(toPoint);
        printf("Composed: ");
        mat4_print(mat4_mul(toPoint, mat4_mul(rotation, toOrigin)));
    }
}