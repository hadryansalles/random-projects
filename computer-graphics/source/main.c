/*
 * @author Hadryan Salles
 */

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gl/gl.h>
#include <math.h>

#include "core.h"
#include "cglm.h"

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    uint32_t screenWidth = 1280;
    uint32_t screenHeight = 720;
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

    unsigned int shaderProgram = shader_create("source/main.vert", "source/main.frag");
    int modelLocation = glGetUniformLocation(shaderProgram, "model");

    Mesh mesh = mesh_read("cube.obj");

    vec3 translation = vec3_new(0, 0, 0);
    vec3 scale = vec3_new(1.0, 1.0, 1.0);
    vec3 rotation = vec3_zero();
    float camDist = 4;
    float camFov = 45;
    vec3 camPos = vec3_new(0, 0, camDist);
    vec3 camRotation = vec3_new(0, 0, 0);
    vec3 camTarget = vec3_zero();
    vec3 camUp = vec3_new(0, 1, 0);

    int viewMode = 0;
    int viewDeboucing = 0;
    int manipulationMode = 0;
    int transformMode = 0;
    int cameraManipulationMode = 0;

    printf("Commands --------\n");
    printf("A - Backward\n");
    printf("D - Forward\n");
    printf("Right\n");
    printf("Left\n");
    printf("Up\n");
    printf("Down\n\n");
    printf("Modes --------\n");
    printf("O - Observer manipulation\n");
    printf("M - Model manipulation\n");
    printf("T - Translation\n");
    printf("R - Rotation\n");
    printf("E - Scale\n");
    printf("P - Perspective projection\n");
    printf("G - Ortho(G)raphic projection\n");
    printf("1 - FPS camera\n");
    printf("2 - Orbit camera\n");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }

        uint32_t newWidth = screenWidth;
        uint32_t newHeight = screenHeight;
        glfwGetWindowSize(window, &newWidth, &newHeight);
        if (newWidth != screenWidth || newHeight != screenHeight) {
            screenWidth = newWidth;
            screenHeight = newHeight;
            glViewport(0, 0, screenWidth, screenHeight);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
        glUseProgram(shaderProgram);

        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
            if (!viewDeboucing) {
                viewMode = (viewMode + 1) % 2;
                if (viewMode == 1) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                } else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
            viewDeboucing = 1;
        } else {
            viewDeboucing = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
            manipulationMode = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            manipulationMode = 1;
            if (transformMode == 2) {
                transformMode = 0;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
            transformMode = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            transformMode = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            cameraManipulationMode = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            cameraManipulationMode = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
            camFov += 0.1;
        }
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
            camFov -= 0.1;
        }
        if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
            camDist += 0.1;
        }
        if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
            camDist -= 0.1;
        }
        if (manipulationMode == 0 && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            transformMode = 2;
        }
        vec3 delta = vec3_zero();
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            delta.y += 1;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            delta.y -= 1;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            delta.x += 1;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            delta.x -= 1;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            delta.z += 1;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            delta.z -= 1;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            translation = vec3_zero();
            scale = vec3_new(5, 5, 5);
            rotation = vec3_zero();
        }
        if (manipulationMode == 0) {
            if (transformMode == 0) {
                translation = vec3_add(translation, vec3_mul(delta, 0.03));
            } else if (transformMode == 1) {
                rotation = vec3_add(rotation, vec3_mul(vec3_new(delta.y, delta.x, -delta.z), 1.5));
            } else if (transformMode == 2) {
                delta = vec3_add(vec3_new(1, 1, 1), vec3_mul(delta, 0.02));
                scale.x *= delta.x;
                scale.y *= delta.y;
                scale.z *= delta.z;
            }
        } else if (manipulationMode == 1) {
            if (transformMode == 0) {
                if (cameraManipulationMode == 0) {
                    camPos = vec3_add(camPos, vec3_mul(delta, 0.03));
                } else {
                    camTarget = vec3_add(camTarget, vec3_mul(delta, 0.03));
                }
            } else if (transformMode == 1) {
                camRotation = vec3_add(camRotation, vec3_mul(vec3_new(-delta.y, delta.x, -delta.z), 1.5));
            }
            if (cameraManipulationMode == 0) {
                camTarget = vec3_add(camPos, vec3_from_vec4(mat4_apply(mat4_from_quat(quat_from_euler(camRotation)), vec4_new(0, 0, -camDist, 0))));
            } else {
                camPos = vec3_add(camTarget, vec3_from_vec4(mat4_apply(mat4_from_quat(quat_inverse(quat_from_euler(camRotation))), vec4_new(0, 0, camDist, 0))));
            }
            camUp = vec3_from_vec4(mat4_apply(mat4_from_quat(quat_from_euler(camRotation)), vec4_new(0, 1, 0, 0)));
        }

        float aspect = screenWidth / (float)screenHeight;
        mat4 projection = mat4_perspective(camFov, aspect, 0.001f, 1000.0f);
        mat4 model = mat4_transform(scale, quat_from_euler(rotation), translation);
        mat4 view = mat4_lookat(camPos, camTarget, camUp);
        mat4 mvp = mat4_mul(model, mat4_mul(view, projection));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, mvp.data);

        mesh_draw(mesh);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    mesh_delete(mesh);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}