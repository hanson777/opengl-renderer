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

	void UploadMesh(Mesh& mesh);
	void UploadTexture(Texture& texture);
	void Upload();

	void BindMesh(const Mesh& mesh);
	void BindMaterial(const Material& material);
	void BindTexture(const Texture& texture);

	void RenderFrame();

	int LoadShader(const std::string& vert, const std::string& frag);
	Shader* GetShaderByIndex(int index);
}
