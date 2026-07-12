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
    float radius;
    float innerCutoff;
    float outerCutoff;
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
uniform Light lights[10];
uniform int lightCount;

vec3 calcSpotlight(Light light, vec3 N, vec3 V) {
    vec3 L = normalize(light.position - fragPos);

    float theta = dot(L, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float spotIntensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    if (spotIntensity <= 0.0) return vec3(0.0);

    // Diffuse
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = Kd * (light.color * diff * texture(material.diffuse, uv).rgb);

    // Specular
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(V, R), 0.0), material.shininess);
    vec3 specular = Ks * (light.color * spec * texture(material.specular, uv).rgb);

    // Attenuation
    float dist = length(light.position - fragPos);
    float attenuation = clamp(1.0 - (dist / light.radius), 0.0, 1.0);
    attenuation *= attenuation;

    return light.intensity * attenuation * spotIntensity * (diffuse + specular);
  }

vec3 calcDirectionalLight(Light light, vec3 N, vec3 V) {
  vec3 L = normalize(-light.direction);

  // Diffuse
  float diff = max(dot(N, L), 0.0);
  vec3 diffuse = Kd * (light.color * diff * texture(material.diffuse, uv).rgb);

  // Specular
  vec3 R = reflect(-L, N);
  float spec = pow(max(dot(V, R), 0.0), material.shininess);
  vec3 specular = Ks * (light.color * spec * texture(material.specular, uv).rgb);

  return light.intensity * (diffuse + specular);
}

vec3 calcPointLight(Light light, vec3 N, vec3 V) {
  vec3 L = normalize(light.position - fragPos);

  // Diffuse
  float diff = max(dot(N, L), 0.0);
  vec3 diffuse = Kd * (light.color * diff * texture(material.diffuse, uv).rgb);

  // Specular
  vec3 R = reflect(-L, N);
  float spec = pow(max(dot(V, R), 0.0), material.shininess);
  vec3 specular = Ks * (light.color * spec * texture(material.specular, uv).rgb);

  // Attenuation
  float dist = length(light.position - fragPos);
  float attenuation = clamp(1.0 - (dist / light.radius), 0.0, 1.0);
  attenuation *= attenuation;

  return light.intensity * attenuation * (diffuse + specular);
}

void main() {
  if (texture(material.diffuse, uv).a < 0.1) discard;

  // vec3 N = texture(material.normal, uv).rgb;
  // N = normalize(TBN * (N * 2) - 1);
  vec3 N = normalize(normal);
  vec3 V = normalize(viewPos - fragPos);

  // Ambient
  vec3 ambient = vec3(0.15) * texture(material.diffuse, uv).rgb;

  vec3 result = ambient;
  for (int i = 0; i < lightCount; i++) {
    if (lights[i].type == 0) { 
      result += calcPointLight(lights[i], N, V);
    } else if (lights[i].type == 1) {
      result += calcDirectionalLight(lights[i], N, V);
    } else if (lights[i].type == 2) {
      result += calcSpotlight(lights[i], N, V);
    }
  }

  result = pow(result, vec3(1.0 / 2.2));

  FragColor = vec4(result, 1.0);
}
