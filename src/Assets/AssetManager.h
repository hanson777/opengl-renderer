#pragma once
#include "Model.h"

namespace AssetManager {
	extern std::vector<Model> g_models;
	extern std::vector<Mesh> g_meshes;

	Mesh* GetMeshByIndex(int index);
}
