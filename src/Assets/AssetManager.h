#pragma once
#include "Model.h"
#include "Mesh.h"
#include "Primitive.h"

namespace AssetManager {
	extern std::vector<Model> g_models;
	extern std::vector<Mesh> g_meshes;
	extern std::vector<MeshData> g_meshData;
	extern std::vector<Primitive> g_primitives;

	int LoadModel(const std::string& filepath); 
	int LoadPrimitive(const PrimitiveType type);
	Model* GetModelByIndex(int index);
	Mesh* GetMeshByIndex(int index);
}
