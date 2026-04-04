#pragma once
#include "SceneObject.h"
#include "../Core/Camera.h"
#include <vector>

namespace Scene {
	extern std::vector<SceneObject> g_sceneObjects;
	extern Camera g_camera;

	void Update(float deltaTime);
}
