#version 330

uniform mat4 mvp;

in vec3 position;
// 2D Texture coordinate.
in vec2 tex_uv;
out vec2 uv;

void main() {
    uv = tex_uv;
    gl_Position = mvp * vec4(position, 1.0);
}
