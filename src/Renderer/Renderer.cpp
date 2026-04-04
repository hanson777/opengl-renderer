#include "Renderer.h"
#include "../Assets/AssetManager.h"
#include "../Core/Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>

namespace Renderer {
    Shader* g_activeShader = nullptr;
    float g_deltaTime = 0.0f;
    float g_lastFrame = 0.0f;

    void Init() {
        glEnable(GL_DEPTH_TEST);
    }

    void BindMesh(Mesh& mesh) {
        glGenVertexArrays(1, &mesh.m_vao);
        glGenBuffers(1, &mesh.m_vbo);
        glGenBuffers(1, &mesh.m_ebo);

        glBindVertexArray(mesh.m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.m_vbo);

        glBufferData(GL_ARRAY_BUFFER, mesh.m_vertices.size() * sizeof(Vertex), &mesh.m_vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.m_indices.size() * sizeof(uint32_t), &mesh.m_indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        glBindVertexArray(0);
    }

    void BindTexture(Texture& texture) {
        glGenTextures(1, &texture.m_id);
        glBindTexture(GL_TEXTURE_2D, texture.m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, texture.GetInternalFormat(), texture.GetWidth(), texture.GetHeight(), 0, texture.GetFormat(), GL_UNSIGNED_BYTE, texture.GetRawData());
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        std::cout << "Texture ID: " << texture.m_id << std::endl;
    }

    void BindMaterial(const Material& material) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material.m_diffuseMap.GetId());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material.m_specularMap.GetId());
    }

    void RenderFrame() {
        float currentFrame = glfwGetTime();
        g_deltaTime = currentFrame - g_lastFrame;
        g_lastFrame = currentFrame;

        Scene::Update(g_deltaTime);

        glClearColor(0.38f, 0.59f, 0.94f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (SceneObject& sceneObject : Scene::g_sceneObjects) {

            Model* model = sceneObject.m_model;
            for (int i = 0; i < model->m_meshIndices.size(); i++) {

                int meshIndex = model->m_meshIndices[i];
                Mesh* mesh = AssetManager::GetMeshByIndex(meshIndex);

                const Material& mat = (mesh->m_materialId != -1) ? model->GetMaterials()[mesh->m_materialId] : model->GetDefaultMaterial();

                BindMaterial(mat);

                g_activeShader->use();

                glm::mat4 projection = glm::perspective(glm::radians(Scene::g_camera.GetFov()), (float)1920 / (float)1080, 0.1f, 500.0f);

                g_activeShader->setMat4("view", Scene::g_camera.GetViewMatrix());
                g_activeShader->setMat4("projection", projection);

                g_activeShader->setMat4("model", sceneObject.GetModelMatrix());

                g_activeShader->setVec3("Ka", mat.m_ambient);
                g_activeShader->setVec3("Kd", mat.m_diffuse);
                g_activeShader->setVec3("Ks", mat.m_specular);

                g_activeShader->setFloat("material.shininess", mat.m_shininess);

                g_activeShader->setInt("material.diffuse", 0);
                g_activeShader->setInt("material.specular", 1);

                glBindVertexArray(mesh->m_vao);
                glDrawElements(GL_TRIANGLES, mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
    }
}
