#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "src/Core/Shader.h"
#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include "src/Core/Camera.h"
#include "src/Core/Window.h"
#include "src/Assets/Model.h"
#include "src/Assets/AssetManager.h"
#include "src/Renderer/Renderer.h"
#include "src/Scene/Scene.h"

constexpr int SCR_WIDTH = 1920;
constexpr int SCR_HEIGHT = 1080;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos = glm::vec3(3.0f, 2.0f, 1.0f);

void mouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
    if (firstMouse) {
        lastX = xposIn;
        lastY = yposIn;
        firstMouse = false;
    }
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    Scene::g_camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow* window) {
    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Scene::g_camera.ProcessKeyboard(Camera::Forward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Scene::g_camera.ProcessKeyboard(Camera::Backward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Scene::g_camera.ProcessKeyboard(Camera::Left, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Scene::g_camera.ProcessKeyboard(Camera::Right, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        Scene::g_camera.ProcessKeyboard(Camera::Up, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        Scene::g_camera.ProcessKeyboard(Camera::Down, deltaTime);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Scene::g_camera.ProcessMouseScroll(yoffset);
}

int main() {
    Window window(SCR_WIDTH, SCR_HEIGHT, "hi");
    glfwMaximizeWindow(window.window());
    glfwSetCursorPosCallback(window.window(), mouseCallback);
    glfwSetScrollCallback(window.window(), scrollCallback);

    Renderer::Init();

	Shader lightTargetShader("shaders/phongShader.vert", "shaders/phongShader.frag");
    Shader lightSourceShader("shaders/blankShader.vert", "shaders/blankShader.frag");

    Model cube("obj/cube/cube.obj");
    Model sponza("obj/sponza/sponza.obj");

    // Bind all meshes via the Renderer
    for (Mesh& mesh : AssetManager::g_meshes) {
        Renderer::BindMesh(mesh);
    }

    // Bind all textures via the Renderer
    for (Material& mat : cube.GetMaterials()) {
        if (mat.m_diffuseMap.m_data.empty() == false) Renderer::BindTexture(mat.m_diffuseMap);
        if (mat.m_specularMap.m_data.empty() == false) Renderer::BindTexture(mat.m_specularMap);
    }
    if (cube.GetDefaultMaterial().m_diffuseMap.m_data.empty() == false) Renderer::BindTexture(cube.GetDefaultMaterial().m_diffuseMap);
    if (cube.GetDefaultMaterial().m_specularMap.m_data.empty() == false) Renderer::BindTexture(cube.GetDefaultMaterial().m_specularMap);

    for (Material& mat : sponza.GetMaterials()) {
        if (mat.m_diffuseMap.m_data.empty() == false) Renderer::BindTexture(mat.m_diffuseMap);
        if (mat.m_specularMap.m_data.empty() == false) Renderer::BindTexture(mat.m_specularMap);
    }
    if (sponza.GetDefaultMaterial().m_diffuseMap.m_data.empty() == false) Renderer::BindTexture(sponza.GetDefaultMaterial().m_diffuseMap);
    if (sponza.GetDefaultMaterial().m_specularMap.m_data.empty() == false) Renderer::BindTexture(sponza.GetDefaultMaterial().m_specularMap);

    Renderer::g_activeShader = &lightTargetShader;

    while (!glfwWindowShouldClose(window.window())) {
        window.beginFrame();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window.window());

        glClearColor(0.38f, 0.59f, 0.94f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = Scene::g_camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(Scene::g_camera.GetFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);

        // Light source cube
        lightSourceShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.25f));
        lightSourceShader.setMat4("model", model);
        lightSourceShader.setMat4("projection", projection);
        lightSourceShader.setMat4("view", view);

        // Draw cube directly (light source uses different shader)
        for (int idx : cube.m_meshIndices) {
            Mesh* mesh = AssetManager::GetMeshByIndex(idx);
            glBindVertexArray(mesh->m_vao);
            glDrawElements(GL_TRIANGLES, mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        // Sponza scene via Renderer
        lightTargetShader.use();
        lightTargetShader.setVec3("light.position", lightPos);
        lightTargetShader.setVec3("light.ambient", glm::vec3(0.2f));
        lightTargetShader.setVec3("light.diffuse", glm::vec3(1.0f));
        lightTargetShader.setVec3("light.specular", glm::vec3(1.0f));
        lightTargetShader.setVec3("viewPos", Scene::g_camera.GetPosition());

        // Draw sponza meshes
        for (int i = 0; i < sponza.m_meshIndices.size(); i++) {
            int meshIndex = sponza.m_meshIndices[i];
            Mesh* mesh = AssetManager::GetMeshByIndex(meshIndex);

            const Material& mat = (mesh->m_materialId != -1) ? sponza.GetMaterials()[mesh->m_materialId] : sponza.GetDefaultMaterial();

            Renderer::BindMaterial(mat);

            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.1f));
            lightTargetShader.setMat4("projection", projection);
            lightTargetShader.setMat4("view", view);
            lightTargetShader.setMat4("model", model);

            lightTargetShader.setVec3("Ka", mat.m_ambient);
            lightTargetShader.setVec3("Kd", mat.m_diffuse);
            lightTargetShader.setVec3("Ks", mat.m_specular);
            lightTargetShader.setFloat("material.shininess", mat.m_shininess);
            lightTargetShader.setInt("material.diffuse", 0);
            lightTargetShader.setInt("material.specular", 1);

            glBindVertexArray(mesh->m_vao);
            glDrawElements(GL_TRIANGLES, mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        window.endFrame();
    }

    glfwTerminate();
    return 0;
}
