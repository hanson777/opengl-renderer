#include "AssetManager.h"
#include "Model.h"
#include "Mesh.h"
#include <vector>
#include <iostream>

namespace AssetManager {
	std::vector<Model> g_models;
	std::vector<Mesh> g_meshes;

	int LoadModel(const std::string& filepath) {
		int index = g_models.size();
		g_models.emplace_back(filepath);
		return index;
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
