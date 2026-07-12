#include "AssetManager.h"
#include "Model.h"
#include "Mesh.h"
#include "Primitive.h"
#include <vector>
#include <iostream>

namespace AssetManager {
	std::vector<Model> g_models;
	std::vector<Mesh> g_meshes;
	std::vector<MeshData> g_meshData;
	std::vector<Primitive> g_primitives;
	std::vector<Mesh> g_primitiveMeshes;

	int LoadModel(const std::string& filepath) {
		int index = g_models.size();
		g_models.emplace_back(filepath);
		return index;
	}

	int LoadPrimitive(PrimitiveType type) {
		int index = g_primitives.size();
		if (type == PrimitiveType::Quad) {
			g_primitives.push_back(CreateQuad());
		}
		return index;
	}

	Primitive CreateQuad() {
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
		return { data, -1 };
	}

	Model* GetModelByIndex(int index) {
		if (index < 0 || index >= g_models.size()) {
			std::cout << "[ERROR::ASSET_MANAGER] GetModelByIndex out of bounds" << std::endl;
			return nullptr;
		}
		return &g_models[index];
	}

	Mesh* GetMeshByIndex(int index) {
		if (index < 0 || index >= g_meshes.size()) {
			std::cout << "[ERROR::ASSET_MANAGER] GetMeshByIndex out of bounds" << std::endl;
			return nullptr;
		}
		return &g_meshes[index];
	}

	Mesh* GetPrimitiveMeshByIndex(int index) {
		if (index < 0 || index >= g_primitiveMeshes.size()) {
			std::cout << "[ERROR::ASSET_MANAGER] GetPrimitiveMeshByIndex out of bounds" << std::endl;
			return nullptr;
		}
		return &g_primitiveMeshes[index];
	}

	Primitive* GetPrimitiveByIndex(int index) {
		if (index < 0 || index >= g_primitives.size()) {
			std::cout << "[ERROR::ASSET_MANAGER] GetPrimitiveByIndex out of bounds" << std::endl;
			return nullptr;
		}
		return &g_primitives[index];
	}
}
