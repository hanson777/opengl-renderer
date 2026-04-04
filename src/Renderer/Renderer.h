#pragma once
#include "../Scene/Scene.h"
#include "../Assets/AssetManager.h"
#include "../Assets/Mesh.h"
#include "../Assets/Material.h"
#include "../Assets/Texture.h"
#include "../Core/Shader.h"
#include <string>

namespace Renderer {
	void Init();

	void BindMesh(Mesh& mesh);
	void BindMaterial(const Material& material);
	void BindTexture(Texture& texture);

	void RenderFrame();

	int LoadShader(const std::string& vert, const std::string& frag);
	Shader* GetShaderByIndex(int index);
}
