#pragma once
#include "SceneObject.h"
#include "Light.h"
#include "../Core/Camera.h"
#include <vector>

namespace Scene {
	extern Camera g_camera;
	extern std::vector<SceneObject> g_sceneObjects;
    extern std::vector<Light> g_lights;

	void Update(float deltaTime);
}
