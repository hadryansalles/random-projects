/*
 * @author Hadryan Salles
 * @date   03/09/2024
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
void update_vertices(vec3 pos, float w, float h, float color, vec3* vertices);
vec3 random_speed();

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

    float changing_color = 0.0f;
    float width = 0.2 * 0.5f;
    float height = 0.2 * sqrt(3.0f) / 3.0f;
    vec3 pos = {0.5, 0.5, 0.0};
    vec3 vertices[6];
    vec3 speed = random_speed();

    update_vertices(pos, width, height, changing_color, vertices);
    Mesh mesh = mesh_create(vertices, 3);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
            double mouseX = 0;
            double mouseY = 0;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            int width = 1;
            int height = 1;
            glfwGetWindowSize(window, &width, &height);
            pos.x = mouseX / (float)width;
            pos.y = mouseY / (float)height;
        } else {
            pos = vec3_add(pos, speed);
            if ((pos.x - width) < 0.0f || (pos.x + width) > 1.0f) {
                speed.x *= -1.0f;
                pos.x = max(0.0f + width, min(1.0f - width, pos.x));
            }
            if ((pos.y - height) < 0.0f || (pos.y + height) > 1.0f) {
                speed.y *= -1.0f;
                pos.y = max(0.0f + height, min(1.0f - height, pos.y));
            }
        }
        update_vertices(pos, width, height, changing_color, vertices);
        mesh_update(mesh, vertices, 3);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        mesh_draw(mesh);

        changing_color += 0.01f;
        if (changing_color > 1.0f) {
            changing_color = 0.0f;
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

void update_vertices(vec3 mousePos, float w, float h, float color, vec3* vertices) {
    float sqrt3 = sqrt(3.0f);

    vertices[1].y = mousePos.x;
    vertices[3].y = mousePos.x; 
    vertices[5].y = mousePos.x; 

    vertices[1].x = mousePos.y; 
    vertices[3].x = mousePos.y; 
    vertices[5].x = mousePos.y; 

    vertices[1].z = color;
    vertices[3].z = color;
    vertices[5].z = color;

    mousePos.x = mousePos.x * 2.0f - 1.0f;
    mousePos.y = 1.0f - mousePos.y * 2.0f;

    vertices[0].x = mousePos.x; 
    vertices[0].y = mousePos.y + 2*h;
    vertices[2].x = mousePos.x - 2*w;
    vertices[2].y = mousePos.y - 2*h;
    vertices[4].x = mousePos.x + 2*w;
    vertices[4].y = mousePos.y - 2*h;

    vertices[0].z = 0;
    vertices[2].z = 0;
    vertices[4].z = 0;
}

vec3 random_speed() {
    float x = -1.0f + 2.0f * rand() / ((float)RAND_MAX + 1); 
    float y = -1.0f + 2.0f * rand() / ((float)RAND_MAX + 1); 
    float mod = sqrt(x * x + y * y);
    float speed = 0.004f;
    vec3 v = { speed * x / mod, speed * y / mod, 0.0f };
    return v;
}
