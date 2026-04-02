#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader/shader.h"
#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include "camera/camera.h"
#include "window/window.h"
#include "model/model.h"

constexpr int SCR_WIDTH = 1920;
constexpr int SCR_HEIGHT = 1080;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera camera(cameraPos, cameraUp);

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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow* window) {
    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Forward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Backward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Left, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Right, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Up, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Down, deltaTime);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

int main() {
    Window window(SCR_WIDTH, SCR_HEIGHT, "hi");
    glfwMaximizeWindow(window.window());
    glfwSetCursorPosCallback(window.window(), mouseCallback);
    glfwSetScrollCallback(window.window(), scrollCallback);

    glEnable(GL_DEPTH_TEST);

	Shader lightTargetShader("shaders/phongShader.vert", "shaders/phongShader.frag");
    Shader lightSourceShader("shaders/blankShader.vert", "shaders/blankShader.frag");

    Model cube("obj/cube/cube.obj");
    // Model suzanne("obj/suzanne/suzanne.obj");
    Model sponza("obj/sponza/sponza.obj");
    while (!glfwWindowShouldClose(window.window())) {
        window.beginFrame();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window.window());

        glClearColor(0.38f, 0.59f, 0.94f, 1.0f);
        // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 view = camera.View();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Fov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);

        lightSourceShader.use();

        float t = glfwGetTime();
        float r = 1.5f;
        float b = 5.0f;
        lightPos = glm::vec3(sin(b*t) * r, 1.0f, cos(b*t) * r); 
        //lightPos = glm::vec3(1.0f, 1.5f, 1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.25f)); // a smaller cube
        lightSourceShader.setMat4("model", model);
        lightSourceShader.setMat4("projection", projection);
        lightSourceShader.setMat4("view", view);

        cube.Draw(lightSourceShader);

        lightTargetShader.use();

        // Material properties
        lightTargetShader.setFloat("material.shininess", 0.4f * 128.0f);

        // Light properties
        lightTargetShader.setVec3("light.position", lightPos);
        lightTargetShader.setVec3("light.ambient", glm::vec3(0.2f));
        lightTargetShader.setVec3("light.diffuse", glm::vec3(1.0f));
        lightTargetShader.setVec3("light.specular", glm::vec3(1.0f));

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f));
        lightTargetShader.setMat4("projection", projection);
        lightTargetShader.setMat4("view", view);
        lightTargetShader.setMat4("model", model);
        lightTargetShader.setVec3("viewPos", camera.Position());

        sponza.Draw(lightTargetShader);

        window.endFrame();
    }

    glfwTerminate();
    return 0;
}
