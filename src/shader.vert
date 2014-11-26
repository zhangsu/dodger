#version 330

uniform mat4 mvp;

in vec3 position;
out vec2 uv;

void main() {
    uv = position.xz;
    gl_Position = mvp * vec4(position, 1.0);
}
