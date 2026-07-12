#pragma once
#include "Mesh.h"

enum class PrimitiveType { Quad };

struct Primitive {
	MeshData data;
	int shaderIndex = -1;
	int meshIndex = -1;
};