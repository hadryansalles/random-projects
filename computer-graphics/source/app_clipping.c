/*
 * @author Hadryan Salles
 */

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "core.h"
#include "cglm.h"

#define MAX_POINTS 1024

int find_closest_point(vec3 mouse, const vec3* points, int count, float max_distance) {
    float dist = 1000000.0f;
    int closest = -1;
    for (int i = 0; i < count; i++) {
        vec3 v = vec3_sub(points[i], mouse);
        float curr = vec3_dot(v, v);
        if (curr < dist) {
            dist = curr;
            closest = i;
        }
    }
    return dist > max_distance ? -1 : closest; 
}

vec3 intersection_point(vec3 p0, vec3 p1, vec3 v0, vec3 v1) {
    vec3 u = {p1.x - p0.x, p1.y - p0.y, p1.z - p0.z};
    vec3 v = {v1.x - v0.x, v1.y - v0.y, v1.z - v0.z};

    float det = u.x * v.y - u.y * v.x;

    if (det == 0.0f) {
        return p0;
    }

    float t = ((v0.x - p0.x) * v.y - (v0.y - p0.y) * v.x) / det;
    return vec3_add(p0, vec3_mul(u, t));
}

int inside(vec3 p0, vec3 p1, vec3 v) {
    vec3 c = vec3_cross(vec3_sub(p1, p0), vec3_sub(v, p0));
    return c.z >= 0;
}

void clip(const vec3* vertices, int vertexCount, const vec3* polygon, int polygonCount, vec3* clipVertices, int* clipCount) {
    vec3 outputList[MAX_POINTS];

    for (int i = 0; i < vertexCount; ++i) {
        outputList[i] = vertices[i];
    }
    int outputCount = vertexCount;

    for (int i = 1; i < polygonCount + 1; i++) {
        vec3 p1 = polygon[i % polygonCount];
        vec3 p0 = polygon[i - 1];

        vec3 inputList[MAX_POINTS];
        memcpy(inputList, outputList, MAX_POINTS * sizeof(vec3));
        int inputCount = outputCount;
        outputCount = 0;
        
        for (int j = 1; j < inputCount + 1; j++) {
            vec3 v1 = inputList[j % inputCount];
            vec3 v0 = inputList[j - 1];
            vec3 point = intersection_point(p0, p1, v0, v1);

            int current = inside(p0, p1, v1);
            int prev = inside(p0, p1, v0);

            if (current) {
                if (!prev) {
                    outputList[outputCount++] = point;
                }
                outputList[outputCount++] = v1;
            } else if (prev) {
                outputList[outputCount++] = point;
            }
        }
    }

    *clipCount = outputCount;
    for (int i = 0; i < outputCount; i++) {
        clipVertices[i] = outputList[i];
    }
}

int app_clipping(int argc, const char* argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int32_t screenWidth = 1280;
    int32_t screenHeight = 720;
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "CSV40 Hadryan Salles", NULL, NULL);
    if (window == NULL) {
        printf("Failed to initialize GLFW\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    vec3 points[MAX_POINTS];
    vec3 vertices[MAX_POINTS];
    vec3 clipVertices[MAX_POINTS];
    int count = 0;
    int moving = -1;

    Mesh canvas = mesh_create_vertices(NULL, 0);
    Mesh polygon = mesh_create_vertices(NULL, 0);
    Mesh clipped = mesh_create_vertices(NULL, 0);

    unsigned int shaderProgram = shader_create("source/app_clipping.vert", "source/app_clipping.frag");
    int colorLocation = glGetUniformLocation(shaderProgram, "color");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glPointSize(15.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineWidth(10.0f);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }

        int newWidth = screenWidth;
        int newHeight = screenHeight;
        glfwGetWindowSize(window, &newWidth, &newHeight);
        if (newWidth != screenWidth || newHeight != screenHeight) {
            screenWidth = newWidth;
            screenHeight = newHeight;
            glViewport(0, 0, screenWidth, screenHeight);
        }

        int mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS ? 1 : 0;
        vec3 mouse = vec3_zero();
        double mouse_x;
        double mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        mouse.x = 2.0f * mouse_x / screenWidth - 1.0f;
        mouse.y = -2.0f * mouse_y / screenHeight + 1.0f;

        if (moving != -1) {
            if (mousePressed) {
                points[moving] = mouse;
            } else {
                moving = -1;
            }
        } else {
            if (mousePressed) {
                moving = find_closest_point(mouse, points, count, 0.03f);
                if (moving == -1) {
                    moving = count++;
                }
                points[moving] = mouse;
                printf("points[%d] = %.3f %3f\n", moving, mouse.x, mouse.y);
            } else {
                moving = -1;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
        glUseProgram(shaderProgram);

        if (count >= 2) {
            vec3 p00 = vec3_min(points[0], points[1]);
            vec3 p11 = vec3_max(points[0], points[1]);
            vec3 p10 = vec3_new(p11.x, p00.y, 0);
            vec3 p01 = vec3_new(p00.x, p11.y, 0);
            const vec3 canvasPoints[] = {p00, p10, p11, p01, p00};
            mesh_update_vertices(&canvas, canvasPoints, 5);
            if (count >= 5) {
                memcpy(vertices, &points[2], (count - 2) * sizeof(vec3));
                vertices[count - 2] = points[2];
                mesh_update_vertices(&polygon, &vertices, count - 1);
                int clipCount = 0;
                clip(vertices, count - 2, canvasPoints, 4, clipVertices, &clipCount);
                clipVertices[clipCount++] = clipVertices[0];
                mesh_update_vertices(&clipped, &clipVertices, clipCount);
            }
        }

        glUniform3f(colorLocation, 0.4, 0.4, 0.4);
        mesh_draw_lines(canvas);
        glUniform3f(colorLocation, 0.6, 0.6, 0.6);
        mesh_draw_points(canvas, 0, canvas.vertexCount > 1 ? 4 : 0);

        glUniform3f(colorLocation, 0.4, 0.0, 0.6);
        mesh_draw_lines(polygon);

        glUniform3f(colorLocation, 0.0, 0.7, 0.3);
        mesh_draw_lines(clipped);

        glUniform3f(colorLocation, 0.7, 0.0, 0.9);
        mesh_draw_points(polygon, 0, polygon.vertexCount);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    mesh_delete(canvas);
    mesh_delete(polygon);
    mesh_delete(clipped);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}