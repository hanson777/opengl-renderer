#pragma once
#include "Model.h"
#include "Mesh.h"

namespace AssetManager {
	extern std::vector<Model> g_models;
	extern std::vector<Mesh> g_meshes;

	int LoadModel(const std::string& filepath);
	Model* GetModelByIndex(int index);
	Mesh* GetMeshByIndex(int index);
}
