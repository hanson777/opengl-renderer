#include "AssetManager.h"

std::vector<Model> AssetManager::g_models;
std::vector<Mesh> AssetManager::g_meshes;

Mesh* AssetManager::GetMeshByIndex(int index) {
	if (index < 0 || index >= g_meshes.size()) return nullptr;
	return &g_meshes[index];
}
