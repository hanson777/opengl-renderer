#pragma once
#include "../Scene/Scene.h"
#include "../Assets/AssetManager.h"
#include "../Core/Shader.h"

namespace Renderer {
	extern Shader* g_activeShader;
	extern float g_deltaTime;
	extern float g_lastFrame;

	void Init();
	void RenderFrame();
	void BindMesh(Mesh& mesh);
	void BindMaterial(const Material& material);
	void BindTexture(Texture& texture);
}
