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

void transform_object(Object* object, int mode, vec3 delta);
void transform_camera(Camera* object, int mode, vec3 delta);

int app_t2(int argc, char* argv[]) {
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

    unsigned int shaderProgram = shader_create("source/app_t2.vert", "source/app_t2.frag");
    int modelLocation = glGetUniformLocation(shaderProgram, "model");
    int camPosLocation = glGetUniformLocation(shaderProgram, "camPos");
    int lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
    int diffuseLocation = glGetUniformLocation(shaderProgram, "Kd");
    int specularLocaltion = glGetUniformLocation(shaderProgram, "Ks");
    int textureLocation = glGetUniformLocation(shaderProgram, "texture1");
    int viewProjectionLocation = glGetUniformLocation(shaderProgram, "viewProjection");

    Object object = {
        .mesh = mesh_read_parser("teapot.obj"),
        .position = {0, 0, 0},
        .rotation = {0, 0, 0},
        .size = {2, 2, 2},
        .texture = texture_load("eye.jpg"),
    };

    Camera camera = {
        .position = {0, 0, 3},
        .fov = 60,
        .rotation = quat_from_euler(vec3_new(0, 0, 0)),
    };

    int viewMode = 0;
    int viewDeboucing = 0;
    int transformMode = 0;
    int target = 1;

    float Ks = 0.5;
    float Kd = 0.5;
    vec3 lightPos = vec3_new(0, 2, 0);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glLineWidth(5.0);

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
        int print_mat = 0;
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
            Kd += 0.01;
            print_mat = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
            Kd -= 0.01;
            print_mat = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
            Ks += 0.01;
            print_mat = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
            Ks -= 0.01;
            print_mat = 1;
        }
        Kd = min(max(0.0f, Kd), 1.0f);
        Ks = min(max(0.0f, Ks), 1.0f);
        if (print_mat) {
            printf("Kd=%.3f Ks=%.3f\n", Kd, Ks);
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            target = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
            target = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            target = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            transformMode = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            transformMode = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
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
        if (target == 0) {
            transform_camera(&camera, transformMode, delta);
        } else if (target == 1) {
            transform_object(&object, transformMode, delta);
        }

        float aspect = screenWidth / (float)screenHeight;
        mat4 viewProjection = camera_get_view_projection(&camera, aspect);
        lightPos.x = cos(glfwGetTime()*2.0) * 2;
        lightPos.z = sin(glfwGetTime()*2.0) * 2;

        glUniform1i(textureLocation, object.texture - 1);
        glUniform1f(diffuseLocation, Kd);
        glUniform1f(specularLocaltion, Ks);
        glUniform3f(camPosLocation, camera.position.x, camera.position.y, camera.position.z);
        glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
        glUniformMatrix4fv(viewProjectionLocation, 1, GL_FALSE, viewProjection.data);
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, object_get_model(&object).data);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, object.texture);

        mesh_draw(object.mesh);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    mesh_delete(object.mesh);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void transform_object(Object* object, int mode, vec3 delta) {
    if (mode == 0) {
        object->position = vec3_add(object->position, vec3_mul(delta, 0.03));
    } else if (mode == 1) {
        delta.z = 0;
        object->rotation = vec3_add(object->rotation, vec3_mul(vec3_new(delta.y, delta.x, -delta.z), 1.5));
    } else if (mode == 2) {
        delta = vec3_add(vec3_new(1, 1, 1), vec3_mul(delta, 0.02));
        object->size.x *= delta.x;
        object->size.y *= delta.y;
        object->size.z *= delta.z;
    }
}

void transform_camera(Camera* camera, int mode, vec3 delta) {
    if (mode == 0) {
        vec4 pos = mat4_apply_vec4(mat4_from_quat(camera->rotation), vec4_from_vec3(vec3_mul(delta, 0.03), 1.0));
        camera->position = vec3_add(camera->position, vec3_from_vec4(pos));
    } else if (mode == 1) {
        delta.z = 0;
        vec4 camRotationDelta = quat_from_euler(vec3_mul(vec3_new(-delta.y, delta.x, -delta.z), 1.5));
        camera->rotation = quat_mul(camRotationDelta, camera->rotation);
    } else if (mode == 2) {
        camera->fov += 0.1 * delta.y;
    }
}