#pragma once
#include "Model.h"
#include "Mesh.h"

namespace AssetManager {
	extern std::vector<Model> g_models;
	extern std::vector<Mesh> g_meshes;
	extern std::vector<MeshData> g_meshData;

	extern std::vector<Mesh> g_primitiveMeshes;
	extern std::vector<MeshData> g_primitiveMeshData;

	int LoadModel(const std::string& filepath); 
	int LoadMeshData(const MeshType type);
	MeshData CreateQuad();
	Model* GetModelByIndex(int index);
	Mesh* GetMeshByIndex(int index);
	Mesh* GetPrimitiveMeshByIndex(int index);
}
