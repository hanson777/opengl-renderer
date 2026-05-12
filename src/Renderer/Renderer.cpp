#include "Renderer.h"
#include "../Assets/Model.h"
#include "../Assets/Mesh.h"
#include "../Assets/Material.h"
#include "../Assets/Texture.h"
#include "../Assets/AssetManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneObject.h"
#include "../Scene/Light.h"
#include "../Core/Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>

namespace Renderer {
    std::vector<Shader> g_shaders;
    float g_deltaTime = 0.0f;
    float g_lastFrame = 0.0f;

    void Init() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        g_lastFrame = glfwGetTime();
    }

    int LoadShader(const std::string& vert, const std::string& frag) {
        int index = g_shaders.size();
        g_shaders.emplace_back(vert, frag);
        return index;
    }

    void UploadMesh(Mesh& mesh, const MeshData& data) {
        mesh.indexCount = data.indices.size();
        mesh.materialId = data.materialId;

        glGenVertexArrays(1, &mesh.vao);
        glGenBuffers(1, &mesh.vbo);
        glGenBuffers(1, &mesh.ebo);

        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

        glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(Vertex), &data.vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(uint32_t), &data.indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
        // vertex tangent 
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

        glBindVertexArray(0);
    }

    void BindMesh(const Mesh& mesh) {
        glBindVertexArray(mesh.vao);
    }

    void UploadTexture(Texture& texture) {
        glGenTextures(1, &texture.id);
        glBindTexture(GL_TEXTURE_2D, texture.id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, texture.internalFormat, texture.width, texture.height, 0, texture.format, GL_UNSIGNED_BYTE, texture.GetRawData());
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void BindMaterial(const Material& material) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material.diffuseMap.id);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material.specularMap.id);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, material.normalMap.id);
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
            UploadTexture(model.GetDefaultMaterial().diffuseMap);
            UploadTexture(model.GetDefaultMaterial().specularMap);
            for (Material& mat : model.GetMaterials()) {
                if (!mat.diffuseMap.data.empty())
                    UploadTexture(mat.diffuseMap);
                if (!mat.specularMap.data.empty())
                    UploadTexture(mat.specularMap);
                if (!mat.normalMap.data.empty())
                    UploadTexture(mat.normalMap);
            }
        }
    }

    void RenderFrame() {
        float currentFrame = glfwGetTime();
        g_deltaTime = currentFrame - g_lastFrame;
        g_lastFrame = currentFrame;

        // glClearColor(0.38f, 0.59f, 0.94f, 1.0f);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = Scene::g_camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(Scene::g_camera.GetFov()), 1920.0f / 1080.0f, 0.1f, 500.0f);

        Shader* currentShader = nullptr;

        for (SceneObject& sceneObject : Scene::g_sceneObjects) {

            glm::mat4 modelMatrix = sceneObject.GetModelMatrix();

            Shader* shader = GetShaderByIndex(sceneObject.m_shaderIndex);
            if (shader != currentShader) {
                currentShader = shader;
                shader->Use();
                shader->SetMat4("view", view);
                shader->SetMat4("projection", projection);
                shader->SetVec3("viewPos", Scene::g_camera.GetPosition());
                shader->SetInt("lightCount", Scene::g_lights.size());

                for (int i = 0; i < Scene::g_lights.size(); i++) {
                    const Light& light = Scene::g_lights[i];
                    if (light.type == LightType::Spot) {
                        shader->SetVec3("lights[" + std::to_string(i) + "].position", Scene::g_camera.GetPosition());
                        shader->SetVec3("lights[" + std::to_string(i) + "].direction", Scene::g_camera.GetFront());
                        shader->SetFloat("lights[" + std::to_string(i) + "].innerCutoff", glm::cos(light.innerCutoff));
                        shader->SetFloat("lights[" + std::to_string(i) + "].outerCutoff", glm::cos(light.outerCutoff));
                        shader->SetVec3("lights[" + std::to_string(i) + "].color", light.color);
                        shader->SetFloat("lights[" + std::to_string(i) + "].intensity", light.intensity);
                        shader->SetFloat("lights[" + std::to_string(i) + "].radius", light.radius);
                    } else {
                        shader->SetVec3("lights[" + std::to_string(i) + "].position", light.position);
                        shader->SetVec3("lights[" + std::to_string(i) + "].direction", light.direction);
                        shader->SetVec3("lights[" + std::to_string(i) + "].color", light.color);
                        shader->SetFloat("lights[" + std::to_string(i) + "].intensity", light.intensity);
                        shader->SetFloat("lights[" + std::to_string(i) + "].radius", light.radius);
                    } 
                }
            }

            Model* model = AssetManager::GetModelByIndex(sceneObject.m_modelIndex);
            for (int i = 0; i < model->m_meshIndices.size(); i++) {

                int meshIndex = model->m_meshIndices[i];
                Mesh* mesh = AssetManager::GetMeshByIndex(meshIndex);

                const Material& mat = (mesh->materialId != -1) ? 
                    model->GetMaterials()[mesh->materialId] : model->GetDefaultMaterial();

                BindMaterial(mat);
                BindMesh(*mesh);

                shader->SetMat4("model", modelMatrix);
                shader->SetVec3("Ka", glm::vec3(0.01));
                shader->SetVec3("Kd", mat.diffuse);
                shader->SetVec3("Ks", mat.specular);
                shader->SetFloat("material.shininess", mat.shininess);
                shader->SetInt("material.diffuse", 0);
                shader->SetInt("material.specular", 1);
                shader->SetInt("material.normal", 2);

                glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
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
