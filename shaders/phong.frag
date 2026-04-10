#version 410 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
  sampler2D normal;
};

struct Light {
  int type;
	vec3 position;
  vec3 direction;
  vec3 color;
  float intensity;
};

out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 uv;
in mat3 TBN;

uniform vec3 viewPos;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform Material material;
uniform Light lights[2];

vec3 calcPointLight(Light light, vec3 N, vec3 V) {
  vec3 L = normalize(light.position - fragPos);
  // Ambient 
  vec3 ambient = Ka * texture(material.diffuse, uv).rgb;

  // Diffuse
  float diff = max(dot(N, L), 0.0f);
  vec3 diffuse = Kd * (light.color * diff * texture(material.diffuse, uv).rgb);
  diffuse.x = pow(diffuse.x, 1.0/2.2);
  diffuse.y = pow(diffuse.y, 1.0/2.2);
  diffuse.z = pow(diffuse.z, 1.0/2.2);

  // Specular
  vec3 R = reflect(-L, N);
  float spec = pow(max(dot(V, R), 0.0f), material.shininess);
  vec3 specular = Ks * (light.color * spec * texture(material.specular, uv).rgb);

  return light.intensity * (ambient + diffuse + specular);
}

void main() {
  // vec3 N = texture(material.normal, uv).rgb;
  // N = normalize(TBN * (N * 2) - 1);
  vec3 N = normalize(normal);

  vec3 V = normalize(viewPos - fragPos);
  vec3 result = vec3(0);
  for (int i = 0; i < 2; i++) {
    result += calcPointLight(lights[i], N, V);
  }
  FragColor = vec4(result, 1.0f);
  // FragColor = vec4(normalize(N*0.5 + 0.5), 1.0);
}
