#version 330

uniform mat4 mv;
uniform mat4 mvp;

in vec3 position;
in vec3 normal;
out vec3 unit_normal;
out vec2 uv;

void main() {
    unit_normal = normalize(vec3(inverse(transpose(mv)) * vec4(normal, 0)));
    uv = position.xz;
    gl_Position = mvp * vec4(position, 1.0);
}
