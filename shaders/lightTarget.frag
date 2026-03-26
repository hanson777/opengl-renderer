#version 410 core
struct Material {
	sampler2D diffuse1; // diffuseN for texture loading
	sampler2D specular1; // specularN for texture loading
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

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
  // Ambient 
  vec3 ambient = light.ambient * vec3(texture(material.diffuse1, texCoords));
  
  // Diffuse
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - fragPos);
  float diff = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse1, texCoords));
  
  // Specular
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular1, texCoords));

  vec3 result = (ambient + diffuse + specular);
  FragColor = vec4(result, 1.0);
}
