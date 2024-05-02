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

void print_usage() {
    printf("Usage: ./mesh cube.obj\n\n");
    printf("Commands --------\n");
    printf("Space - Reset transforms\n");
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
    printf("'-' - Decrease FoV\n");
    printf("'+' - Increase FoV\n");
}

int main(int argc, char* argv[]) {

    if (argc < 2 || argv[1][0] == '-') {
        print_usage();
        return 1;
    }

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

    unsigned int shaderProgram = shader_create("source/main.vert", "source/main.frag");
    int modelLocation = glGetUniformLocation(shaderProgram, "model");

    Mesh mesh = mesh_read(argv[1]);

    vec3 translation = vec3_new(0, 0, 0);
    vec3 scale = vec3_new(1.0, 1.0, 1.0);
    vec3 rotation = vec3_zero();
    float camFov = 60;
    vec3 camPos = vec3_new(0, 0, 2);
    vec4 camRotation = quat_identity();

    int viewMode = 0;
    int viewDeboucing = 0;
    int manipulationMode = 0;
    int transformMode = 0;
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

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
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
            camFov += 0.1;
        }
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
            camFov -= 0.1;
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
            scale = vec3_new(1, 1, 1);
            rotation = vec3_zero();
            camFov = 60;
            camPos = vec3_new(0, 0, 2);
            camRotation = quat_identity();
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
                vec4 pos = mat4_apply(mat4_from_quat(camRotation), vec4_from_vec3(vec3_mul(delta, 0.03), 1.0));
                camPos = vec3_add(camPos, vec3_from_vec4(pos));
            } else if (transformMode == 1) {
                vec4 camRotationDelta = quat_from_euler(vec3_mul(vec3_new(-delta.y, delta.x, -delta.z), 1.5));
                camRotation = quat_mul(camRotationDelta, camRotation);
            }
        }

        float aspect = screenWidth / (float)screenHeight;
        mat4 projection = mat4_perspective(camFov, aspect, 0.001f, 1000.0f);
        mat4 model = mat4_transform(scale, quat_from_euler(rotation), translation);
        mat4 camTranslation = mat4_translation(vec3_sub(vec3_zero(), camPos));
        mat4 camRotationMat = mat4_from_quat(quat_inverse(camRotation));
        mat4 view = mat4_mul(camTranslation, camRotationMat);
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