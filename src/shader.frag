#version 330

uniform sampler2D sampler;

in vec3 unit_normal;
in vec2 uv;
out vec4 color;

void main() {
    color = 0.1*vec4(unit_normal, 1) + texture(sampler, uv);
}
