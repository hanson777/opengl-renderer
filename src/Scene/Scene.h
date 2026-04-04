#pragma once
#include "SceneObject.h"
#include "../Core/Camera.h"
#include <vector>

namespace Scene {
	extern Camera g_camera;
	extern std::vector<SceneObject> g_sceneObjects;

	void Update(float deltaTime);
}
