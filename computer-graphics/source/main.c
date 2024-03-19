/*
 * @author Hadryan Salles
 * @date   03/18/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gl/gl.h>
#include <math.h>
#include <time.h>

#include "core.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void log_infos(vec3* vertices, float ratio);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main() {
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

    vec3 vertices[6] = {
        {0, 0, 0},
        {1, 0, 0},
        {0, 0, 0},
        {0, 1, 0},
        {0, 0, 0},
        {0, 0, 1}
    };

    Mesh mesh = mesh_create(vertices, 3);

    int inputCount = 0;

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glLineWidth(10.0);

    int deboucing = 0;
    printf("Click on 3 times on screen to input vertices...\n");

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
            if (deboucing == 1) {
                inputCount++;
            }
            deboucing = 0;
        } else {
            double mouseX = 0;
            double mouseY = 0;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            int width = 1;
            int height = 1;
            glfwGetWindowSize(window, &width, &height);
            int targetVertex = inputCount % 3;
            vertices[2 * targetVertex].x = -1 + 2 * (mouseX / (float) width);
            vertices[2 * targetVertex].y = (-1 + 2 * (mouseY / (float) height)) * -1;
            if (inputCount >= 2) {
                log_infos(vertices, height/(float)width);
            }
            mesh_update(mesh, vertices, 3);
            deboucing = 1;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        if (inputCount > 2) {
            mesh_draw(mesh);
        }

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

void log_infos(vec3* vertices, float ratio) {
    system("cls");
    vec3 p = vertices[0];
    p.y *= ratio;
    vec3 o = vertices[2];
    o.y *= ratio;
    vec3 q = vertices[4];
    q.y *= ratio;
    vec3 u = vec3_sub(p, o);
    vec3 v = vec3_sub(q, o);
    printf("u = %.2f, %.2f\n", u.x, u.y);
    printf("v = %.2f, %.2f\n", v.x, v.y);
    float dot_uv = vec3_dot(u, v);
    float cosTheta = dot_uv / (vec3_length(u) * vec3_length(v));
    float theta = 180.0f * acosf(cosTheta) / K_PI;
    printf("theta (degs) = %.3f\n", theta);
    printf("dot(u, v)    = %.3f\n", dot_uv);
    vec3 cross_uv = vec3_cross(u, v);
    printf("cross(u, v)  = %.2f, %.2f, %.2f\n", cross_uv.x, cross_uv.y, cross_uv.z);
}

