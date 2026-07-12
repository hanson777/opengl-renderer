#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 a_uv;
layout (location = 3) in vec3 aTangent;

out vec2 uv;

void main() {
	gl_Position = vec4(aPos, 1.0);
  uv = a_uv;
}
