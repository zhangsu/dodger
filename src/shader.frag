#version 330

uniform sampler2D sampler;

in vec2 uv;
out vec4 color;

void main() {
      color = texture(sampler, uv);
}
