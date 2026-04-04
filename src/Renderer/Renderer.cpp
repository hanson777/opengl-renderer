#include "Renderer.h"
#include "../Assets/Model.h"
#include "../Assets/Mesh.h"
#include "../Assets/Material.h"
#include "../Assets/Texture.h"
#include "../Assets/AssetManager.h"
#include "../Core/Shader.h"
#include "../Input/Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>

namespace Renderer {
    std::vector<Shader> g_shaders;
    float g_deltaTime = 0.0f;
    float g_lastFrame = 0.0f;

    void Init() {
        glEnable(GL_DEPTH_TEST);
        g_lastFrame = glfwGetTime();
    }

    int LoadShader(const std::string& vert, const std::string& frag) {
        int index = g_shaders.size();
        g_shaders.emplace_back(vert, frag);
        return index;
    }

    void UploadMesh(Mesh& mesh, const MeshData& data) {
        mesh.m_indexCount = data.m_indices.size();
        mesh.m_materialId = data.m_materialId;

        glGenVertexArrays(1, &mesh.m_vao);
        glGenBuffers(1, &mesh.m_vbo);
        glGenBuffers(1, &mesh.m_ebo);

        glBindVertexArray(mesh.m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.m_vbo);

        glBufferData(GL_ARRAY_BUFFER, data.m_vertices.size() * sizeof(Vertex), &data.m_vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.m_indices.size() * sizeof(uint32_t), &data.m_indices[0], GL_STATIC_DRAW);

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

    void BindMesh(const Mesh& mesh) {
        glBindVertexArray(mesh.m_vao);
    }

    void UploadTexture(Texture& texture) {
        glGenTextures(1, &texture.m_id);
        glBindTexture(GL_TEXTURE_2D, texture.m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, texture.GetInternalFormat(), texture.GetWidth(), texture.GetHeight(), 0, texture.GetFormat(), GL_UNSIGNED_BYTE, texture.GetRawData());
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void BindMaterial(const Material& material) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material.m_diffuseMap.GetId());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material.m_specularMap.GetId());
    }

    void Upload() {
        for (MeshData& data : AssetManager::g_meshData) {
            Mesh mesh;
            UploadMesh(mesh, data);
            AssetManager::g_meshes.push_back(mesh);
        }
        AssetManager::g_meshData.clear();
        AssetManager::g_meshData.shrink_to_fit();
        for (Model& model : AssetManager::g_models) {
            UploadTexture(model.GetDefaultMaterial().m_diffuseMap);
            UploadTexture(model.GetDefaultMaterial().m_specularMap);
            for (Material& mat : model.GetMaterials()) {
                if (!mat.m_diffuseMap.m_data.empty())
                    UploadTexture(mat.m_diffuseMap);
                if (!mat.m_specularMap.m_data.empty())
                    UploadTexture(mat.m_specularMap);
            }
        }
    }

    void RenderFrame() {
        float currentFrame = glfwGetTime();
        g_deltaTime = currentFrame - g_lastFrame;
        g_lastFrame = currentFrame;

        glClearColor(0.38f, 0.59f, 0.94f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = Scene::g_camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(Scene::g_camera.GetFov()), 1920.0f / 1080.0f, 0.1f, 500.0f);

        // do it for g_lights once we get to multiple lights
        glm::vec3 lightPos = glm::vec3(0.0f, 2.0f, 0.0f);

        Shader* currentShader = nullptr;

        for (SceneObject& sceneObject : Scene::g_sceneObjects) {
            glm::mat4 modelMatrix = sceneObject.GetModelMatrix();

            Shader* shader = GetShaderByIndex(sceneObject.m_shaderIndex);
            if (shader != currentShader) {
                currentShader = shader;
                shader->use();
                shader->setMat4("view", view);
                shader->setMat4("projection", projection);
                shader->setVec3("viewPos", Scene::g_camera.GetPosition());
                shader->setVec3("light.position", lightPos);
                shader->setVec3("light.ambient", glm::vec3(0.2f));
                shader->setVec3("light.diffuse", glm::vec3(1.0f));
                shader->setVec3("light.specular", glm::vec3(1.0f));
            }

            Model* model = AssetManager::GetModelByIndex(sceneObject.m_modelIndex);
            for (int i = 0; i < model->m_meshIndices.size(); i++) {

                int meshIndex = model->m_meshIndices[i];
                Mesh* mesh = AssetManager::GetMeshByIndex(meshIndex);

                const Material& mat = (mesh->m_materialId != -1) ? 
                    model->GetMaterials()[mesh->m_materialId] : model->GetDefaultMaterial();

                BindMaterial(mat);
                BindMesh(*mesh);

                shader->setMat4("model", modelMatrix);
                shader->setVec3("Ka", mat.m_ambient);
                shader->setVec3("Kd", mat.m_diffuse);
                shader->setVec3("Ks", mat.m_specular);
                shader->setFloat("material.shininess", mat.m_shininess);
                shader->setInt("material.diffuse", 0);
                shader->setInt("material.specular", 1);

                glDrawElements(GL_TRIANGLES, mesh->m_indexCount, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
    }

    Shader* GetShaderByIndex(int index) {
        if (index < 0 || index >= g_shaders.size()) {
            std::cout << "[ERROR::RENDERER] GetShaderByIndex out of bounds" << std::endl;
            return nullptr;
        }
        return &g_shaders[index];
    }
}
