#pragma once
#include "Model.h"
#include "Mesh.h"
#include "Primitive.h"

namespace AssetManager {
	extern std::vector<Model> g_models;
	extern std::vector<Mesh> g_meshes;
	extern std::vector<MeshData> g_meshData;
	extern std::vector<Primitive> g_primitives;
	extern std::vector<Mesh> g_primitiveMeshes;

	int LoadModel(const std::string& filepath); 
	int LoadPrimitive(const PrimitiveType type);
	Primitive CreateQuad();
	Model* GetModelByIndex(int index);
	Mesh* GetMeshByIndex(int index);
	Mesh* GetPrimitiveMeshByIndex(int index);
	Primitive* GetPrimitiveByIndex(int index);
}
