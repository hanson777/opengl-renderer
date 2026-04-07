#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 a_uv;
layout (location = 3) in vec3 aTangent;

out vec3 normal;
out vec3 fragPos;
out vec2 uv;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  fragPos = vec3(model * vec4(aPos, 1.0));
  normal = mat3(transpose(inverse(model))) * aNormal;
  uv = a_uv;

  vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
  vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
  T = normalize(T - dot(T, N) * N);
  vec3 B = cross(N, T);

  TBN = mat3(T, B, N);

  gl_Position = projection * view * vec4(fragPos, 1.0);
}
