/*
 * @author Hadryan Salles
 */

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <time.h>
#include <math.h>

#include "core.h"
#include "cglm.h"

typedef struct Circle_t {
    vec2 center;
    float radius;
} Circle;

vec2 get_cursor(GLFWwindow* window, int height, int scale) {
    double mouseX;
    double mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    vec2 c = {((int)mouseX)/scale, (height - (int)mouseY - 1) / scale};
    return c;
}

void paint(int x, int y, int posLocation, Mesh quadMesh, int scale) {
    glUniform3i(posLocation, x, y, scale);
    mesh_draw(quadMesh);
}

int midpoint_circle_algorithm(Circle c, int iteration, int posLocation, Mesh quadMesh, int scale) {
    if (iteration == 0) {
        paint(c.center.x, c.center.y, posLocation, quadMesh, scale);
    }

    int t1 = c.radius / 16;
    int x = c.radius;
    int y = 0;
    int count = 0;

    while (x >= y) {
        if (count == iteration) {
            paint(c.center.x + x, c.center.y + y, posLocation, quadMesh, scale);
            paint(c.center.x - x, c.center.y + y, posLocation, quadMesh, scale);
            paint(c.center.x + x, c.center.y - y, posLocation, quadMesh, scale);
            paint(c.center.x - x, c.center.y - y, posLocation, quadMesh, scale);
            paint(c.center.x + y, c.center.y + x, posLocation, quadMesh, scale);
            paint(c.center.x - y, c.center.y + x, posLocation, quadMesh, scale);
            paint(c.center.x + y, c.center.y - x, posLocation, quadMesh, scale);
            paint(c.center.x - y, c.center.y - x, posLocation, quadMesh, scale);
            return 0;
        }
        y++;
        t1 += y;
        int t2 = t1 - x;
        if (t2 >= 0) {
            t1 = t2;
            x--;
        }

        count++;
    }

    return 1;
}

int midpoint_circle_algorithm_2(Circle c, int iteration, int posLocation, Mesh quadMesh, int scale) {
    if (iteration == 0) {
        paint(c.center.x, c.center.y, posLocation, quadMesh, scale);
    }
    int x = 0;
    int y = c.radius;
    int d = 3 - 2 * c.radius;

    int count = 0;

    while (y >= x) {
        if (count == iteration) {
            paint(c.center.x + x, c.center.y + y, posLocation, quadMesh, scale);
            paint(c.center.x - x, c.center.y + y, posLocation, quadMesh, scale);
            paint(c.center.x + x, c.center.y - y, posLocation, quadMesh, scale);
            paint(c.center.x - x, c.center.y - y, posLocation, quadMesh, scale);
            paint(c.center.x + y, c.center.y + x, posLocation, quadMesh, scale);
            paint(c.center.x - y, c.center.y + x, posLocation, quadMesh, scale);
            paint(c.center.x + y, c.center.y - x, posLocation, quadMesh, scale);
            paint(c.center.x - y, c.center.y - x, posLocation, quadMesh, scale);
            return 0;
        }

        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }

        count++;
    }

    return 1;
}

int app_circle(int argc, const char* argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int32_t screenWidth = 1024;
    int32_t screenHeight = 1024;
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

    Circle circles[1024];
    uint32_t circleId = 0;
    int lastCircle = 0;
    int lastIter = 0;
    uint32_t scale = 4;

    vec3 quadVertices[] = {{-1, -1, 0}, {1, -1, 0}, {1, 1, 0}, {-1, 1, 0}};
    uint32_t quadIndices[] = {0, 1, 2, 2, 3, 0};
    Mesh quad = mesh_create_non_normals(quadVertices, quadIndices, 4, 2);

    unsigned int shaderProgram = shader_create("source/app_circle.vert", "source/app_circle.frag");
    int posLocation = glGetUniformLocation(shaderProgram, "position");
    int colorLocation = glGetUniformLocation(shaderProgram, "color");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(shaderProgram);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    int dragin = 0;
    clock_t start = clock();

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

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
            vec2 cursor = get_cursor(window, screenHeight, scale);
            if (!dragin) {
                circles[circleId].center = cursor;
            } else {
                circles[circleId].radius = vec2_length(vec2_sub(cursor, circles[circleId].center));
            }
            dragin = 1;
        } else if (dragin) {
            dragin = 0;
            circleId++;
            printf("Current circle %d\n", circleId);
        }
        
        if ((clock() - start > 30 * (CLOCKS_PER_SEC / 1000)) && circleId - lastCircle > 0) {
            if (midpoint_circle_algorithm(circles[lastCircle], lastIter++, posLocation, quad, scale)) {
                printf("Last circle %d\n", lastCircle);
                lastCircle++;
                lastIter = 0;
                vec3 color = vec3_rand();
                glUniform3f(colorLocation, color.x, color.y, color.z);
            }
            start = clock();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    mesh_delete(quad);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}