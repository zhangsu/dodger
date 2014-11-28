#version 330

uniform sampler2D sampler;

in vec2 uv;
out vec4 frag_color;

void main() {
    frag_color = vec4(texture(sampler, uv).rgb, 1.0);
}
