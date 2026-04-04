#pragma once
#include "../Scene/Scene.h"
#include "../Assets/AssetManager.h"
#include "../Core/Shader.h"

namespace Renderer {
	void Init();
	void RenderFrame();
	void BindMesh(Mesh& mesh);
	void BindMaterial(const Material& material);
	void BindTexture(Texture& texture);
}
