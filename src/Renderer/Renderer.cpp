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
    uint32_t g_fbo = -1;
    uint32_t g_rbo = -1;
    uint32_t g_textureColorbuffer = -1;

    void Init() {
        glEnable(GL_DEPTH_TEST);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        CreateFramebuffer();
        g_lastFrame = glfwGetTime();
    }

    int LoadShader(const std::string& vert, const std::string& frag) {
        int index = g_shaders.size();
        g_shaders.emplace_back(vert, frag);
        return index;
    }

    void CreateFramebuffer() {
        glGenFramebuffers(1, &g_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);

        glGenTextures(1, &g_textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D, g_textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_textureColorbuffer, 0);

        glGenRenderbuffers(1, &g_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, g_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, g_rbo);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "[ERROR::RENDERER] framebuffer incomplete: " << status << std::endl;
            exit(1);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

        for (MeshData& data : AssetManager::g_primitiveMeshData) {
            Mesh mesh;
            UploadMesh(mesh, data); 
            AssetManager::g_primitiveMeshes.push_back(mesh);
        }

        AssetManager::g_meshData.clear();
        AssetManager::g_meshData.shrink_to_fit();
        AssetManager::g_primitiveMeshData.clear();
        AssetManager::g_primitiveMeshData.shrink_to_fit();

        for (Model& model : AssetManager::g_models) {
            UploadTexture(model.m_defaultMaterial.diffuseMap);
            UploadTexture(model.m_defaultMaterial.specularMap);
            for (Material& mat : model.m_materials) {
                if (!mat.diffuseMap.data.empty())
                    UploadTexture(mat.diffuseMap);
                if (!mat.specularMap.data.empty())
                    UploadTexture(mat.specularMap);
                if (!mat.normalMap.data.empty())
                    UploadTexture(mat.normalMap);
            }
        }
    }

    void DrawSceneObjects() {
        glm::mat4 view = Scene::g_camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(Scene::g_camera.GetFov()), 1920.0f / 1080.0f, 0.1f, 500.0f);

        Shader* currentShader = nullptr;

        for (SceneObject& sceneObject : Scene::g_sceneObjects) {

            glm::mat4 modelMatrix = sceneObject.GetModelMatrix();

            Shader* shader = GetShaderByIndex(sceneObject.shaderIndex);
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
                    }
                    else {
                        shader->SetVec3("lights[" + std::to_string(i) + "].position", light.position);
                        shader->SetVec3("lights[" + std::to_string(i) + "].direction", light.direction);
                        shader->SetVec3("lights[" + std::to_string(i) + "].color", light.color);
                        shader->SetFloat("lights[" + std::to_string(i) + "].intensity", light.intensity);
                        shader->SetFloat("lights[" + std::to_string(i) + "].radius", light.radius);
                    }
                }
            }

            Model* model = AssetManager::GetModelByIndex(sceneObject.modelIndex);
            for (int i = 0; i < model->m_meshIndices.size(); i++) {

                int meshIndex = model->m_meshIndices[i];
                Mesh* mesh = AssetManager::GetMeshByIndex(meshIndex);

                const Material& mat = (mesh->materialId != -1) ?
                    model->GetMaterials()[mesh->materialId] : model->GetDefaultMaterial();

                BindMaterial(mat);
                glBindVertexArray(mesh->vao);

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

    void DrawScreenSpaceObject() {
        for (ScreenSpaceObject& ssObject : Scene::g_screenSpaceObjects) {
            Shader* shader = Renderer::GetShaderByIndex(ssObject.shaderIndex);
            shader->Use();
            shader->SetInt("screenTexture", 3);

            Mesh* mesh = AssetManager::GetPrimitiveMeshByIndex(ssObject.meshIndex);
            glBindVertexArray(mesh->vao);

            glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }

    void RenderFrame() {
        float currentFrame = glfwGetTime();
        g_deltaTime = currentFrame - g_lastFrame;
        g_lastFrame = currentFrame;

        glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);

        glClearColor(0, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        DrawSceneObjects();
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, g_textureColorbuffer);

        DrawScreenSpaceObject();
    }

    Shader* GetShaderByIndex(int index) {
        if (index < 0 || index >= g_shaders.size()) {
            std::cout << "[ERROR::RENDERER] GetShaderByIndex out of bounds" << std::endl;
            return nullptr;
        }
        return &g_shaders[index];
    }
}
