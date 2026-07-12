#include "Mesh.h"

enum class PrimitiveType { Quad };

struct Primitive {
	MeshData mesh;
	int shaderIdx = -1;
};