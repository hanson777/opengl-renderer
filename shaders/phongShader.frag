#version 410 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

uniform vec3 viewPos;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform Material material;
uniform Light light;

void main() {
  vec3 N = normalize(normal);
  vec3 L = normalize(light.position - fragPos);
  vec3 V = normalize(viewPos - fragPos);
  vec3 R = reflect(-L, N);

  // Ambient 
  vec3 ambient = (light.ambient * texture(material.diffuse, texCoords).rgb) * Ka;
  
  // Diffuse
  float diff = max(dot(N, L), 0.0f);
  vec3 diffuse = (light.diffuse * diff * texture(material.diffuse, texCoords).rgb) * Kd;  
  
  // Specular
  float spec = pow(max(dot(V, R), 0.0f), material.shininess);
  vec3 specular = (light.specular * spec * texture(material.specular, texCoords).rgb) * Ks;

  vec3 result = ambient + diffuse + specular;
  FragColor = vec4(result, 1.0f);
}
