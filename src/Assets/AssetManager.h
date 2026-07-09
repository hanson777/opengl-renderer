#pragma once
#include "Model.h"
#include "Mesh.h"

namespace AssetManager {
	extern std::vector<Model> g_models;
	extern std::vector<Mesh> g_meshes;
	extern std::vector<MeshData> g_meshData;

	int LoadModel(const std::string& filepath); 
	int LoadQuad();
	Model* GetModelByIndex(int index);
	Mesh* GetMeshByIndex(int index);
}
