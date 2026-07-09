#include "AssetManager.h"
#include "Model.h"
#include "Mesh.h"
#include <vector>
#include <iostream>

namespace AssetManager {
	std::vector<Model> g_models;
	std::vector<Mesh> g_meshes;
	std::vector<MeshData> g_meshData;

	int LoadModel(const std::string& filepath) {
		int index = g_models.size();
		g_models.emplace_back(filepath);
		return index;
	}

	int LoadQuad() {
		//float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in ndc
		//	// positions   // uvs
		//	-1.0f,  1.0f,  0.0f, 1.0f, // top left
		//	 1.0f,  1.0f,  0.0f, 0.0f, // top right
		//	-1.0f, -1.0f,  1.0f, 0.0f, // bottom left
		//	 1.0f, -1.0f,  0.0f, 1.0f, // bottom right
		//};

		MeshData data;
		data.vertices = {
			{
				.position = { -1, 1, 0 },  // top left
				.uv       = { 0, 1 },
			},
			{
				.position = { 1, 1, 0 },   // top right
				.uv       = { 0, 0 },
			},
			{
				.position = { -1, -1, 0 }, // bottom left
				.uv       = { 1, 0 },
			},
			{
				.position = { 1, -1, 0 },  // bottom right
				.uv       = { 0, 1 },
			},
		};
		data.indices = { 0, 1, 2, 1, 3, 2 };

		AssetManager::g_meshData.push_back(data);
	}

	Model* GetModelByIndex(int index) {
		if (index < 0 || index >= g_models.size()) {
			std::cout << "[ERROR:ASSET_MANAGER] GetModelByIndex out of bounds" << std::endl;
			return nullptr;
		}
		return &g_models[index];
	}

	Mesh* GetMeshByIndex(int index) {
		if (index < 0 || index >= g_meshes.size()) {
			std::cout << "[ERROR:ASSET_MANAGER] GetMeshByIndex out of bounds" << std::endl;
			return nullptr;
		}
		return &g_meshes[index];
	}
}
