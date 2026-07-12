#version 410 core
out vec4 FragColor;

uniform sampler2D screenTexture;

in vec2 uv;

void main() {
	// FragColor = vec4(0.0, 0.0, 1.0, 1.0);
  FragColor = texture(screenTexture, uv);
}
