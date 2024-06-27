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
    if (argc < 4) {
        printf("Usage: ./mesh2 cube.obj color.jpg normal.jpg\n");
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

    unsigned int shaderProgram = shader_create("source/app_t2.vert", "source/app_t2.frag");
    int modeLocation = glGetUniformLocation(shaderProgram, "mode");
    int modelLocation = glGetUniformLocation(shaderProgram, "model");
    int camPosLocation = glGetUniformLocation(shaderProgram, "camPos");
    int materialLocation = glGetUniformLocation(shaderProgram, "material");
    int albedoLocation = glGetUniformLocation(shaderProgram, "albedoMap");
    int bumpLocation = glGetUniformLocation(shaderProgram, "bumpMap");
    int viewProjectionLocation = glGetUniformLocation(shaderProgram, "viewProjection");

    int lightPosLocation[2] = {
        glGetUniformLocation(shaderProgram, "lightPos0"),
        glGetUniformLocation(shaderProgram, "lightPos1"),
    };
    int lightColorLocation[2] = {
        glGetUniformLocation(shaderProgram, "lightColor0"),
        glGetUniformLocation(shaderProgram, "lightColor1"),
    };

    Object object = {
        .mesh = mesh_read_parser(argv[1]),
        .position = {0, 0, 0},
        .rotation = {0, 0, 0},
        .size = {2, 2, 2},
        .albedoTexture = texture_load(argv[2]),
        .bumpTexture = texture_load(argv[3]),
    };
    Object lightObject = {
        .mesh = mesh_read_parser("sphere.obj"),
        .position = {0, 0, 0},
        .rotation = {0, 0, 0},
        .size = {0.1, 0.1, 0.1},
        .albedoTexture = texture_load("white.jpg"),
        .bumpTexture = texture_load("white.jpg"),
    };

    Camera camera = {
        .position = {0, 0, 3},
        .fov = 60,
        .rotation = quat_from_euler(vec3_new(0, 0, 0)),
    };

    int viewMode = 0;
    int mode = 0;
    int viewDeboucing = 0;
    int transformMode = 0;
    int target = 0;

    vec3 material = vec3_new(0.5f, 0.3f, 128);

    vec3 lightPos0 = vec3_new(0, 0.0, 0);
    vec3 lightPos1 = vec3_new(0, 0.0, 0);
    vec3 lightColor0 = vec3_mul(vec3_new(1.0, 0.8, 0.8), 15.0);
    vec3 lightColor1 = vec3_mul(vec3_new(0.8, 0.8, 1.0), 15.0);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glLineWidth(5.0);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }
        if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS) {
            modeLocation = glGetUniformLocation(shaderProgram, "mode");
            modelLocation = glGetUniformLocation(shaderProgram, "model");
            camPosLocation = glGetUniformLocation(shaderProgram, "camPos");
            materialLocation = glGetUniformLocation(shaderProgram, "material");
            albedoLocation = glGetUniformLocation(shaderProgram, "albedoMap");
            bumpLocation = glGetUniformLocation(shaderProgram, "bumpMap");
            viewProjectionLocation = glGetUniformLocation(shaderProgram, "viewProjection");

            lightPosLocation[0] = glGetUniformLocation(shaderProgram, "lightPos0");
            lightPosLocation[1] = glGetUniformLocation(shaderProgram, "lightPos1");
            
            lightColorLocation[0] = glGetUniformLocation(shaderProgram, "lightColor0");
            lightColorLocation[1] = glGetUniformLocation(shaderProgram, "lightColor1");
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
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
            material.x += 0.01;
            print_mat = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
            material.x -= 0.01;
            print_mat = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
            material.y += 0.01;
            print_mat = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
            material.y -= 0.01;
            print_mat = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
            material.z += 1;
            print_mat = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
            material.z -= 1;
            print_mat = 1;
        }
        if (print_mat) {
            material.x = min(max(0.0f, material.x), 1.0f);
            material.y = min(max(0.0f, material.y), 1.0f);
            material.z = min(max(1.0f, material.z), 256.0f);
            printf("Kd=%.3f Ks=%.3f shininess=%.3f\n", material.x, material.y, material.z);
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            mode = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            mode = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            mode = 2;
        }
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
            target = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
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
        lightPos0.x = sin(glfwGetTime() * 3.0) * cos(glfwGetTime() * 2.0);
        lightPos0.y = sin(glfwGetTime() * 3.0) * sin(glfwGetTime() * 2.0);
        lightPos0.z = cos(glfwGetTime() * 3.0);

        lightPos1.x = sin(glfwGetTime() * 1.0) * cos(glfwGetTime() * 3.0);
        lightPos1.y = sin(glfwGetTime() * 1.0) * sin(glfwGetTime() * 3.0);
        lightPos1.z = cos(glfwGetTime() * 1.0);

        lightPos0 = vec3_mul(vec3_normalize(lightPos0), 4);
        lightPos1 = vec3_mul(vec3_normalize(lightPos1), 4);

        glUniform1i(modeLocation, mode);
        glUniform3fv(materialLocation, 1, material.data);
        glUniform3fv(camPosLocation, 1, camera.position.data);
        glUniform3fv(lightPosLocation[0], 1, lightPos0.data);
        glUniform3fv(lightPosLocation[1], 1, lightPos1.data);
        glUniform3fv(lightColorLocation[0], 1, lightColor0.data);
        glUniform3fv(lightColorLocation[1], 1, lightColor1.data);
        glUniformMatrix4fv(viewProjectionLocation, 1, GL_FALSE, viewProjection.data);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, object_get_model(&object).data);
        glUniform1i(albedoLocation, 0);
        glUniform1i(bumpLocation, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, object.albedoTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, object.bumpTexture);
        mesh_draw(object.mesh);

        vec3 lightMaterial = vec3_new(-1, -1, -1);
        glUniform3fv(materialLocation, 1, lightMaterial.data);

        lightObject.position = lightPos0;
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, object_get_model(&lightObject).data);
        glUniform1i(albedoLocation, 0);
        glUniform1i(bumpLocation, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, lightObject.albedoTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, lightObject.bumpTexture);
        mesh_draw(lightObject.mesh);

        lightObject.position = lightPos1;
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, object_get_model(&lightObject).data);
        glUniform1i(albedoLocation, 0);
        glUniform1i(bumpLocation, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, lightObject.albedoTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, lightObject.bumpTexture);
        mesh_draw(lightObject.mesh);

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