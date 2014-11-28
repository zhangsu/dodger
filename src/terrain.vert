#version 330

uniform mat4 mv;
uniform mat4 mvp;

in vec3 position;
in vec3 normal;
// Untransformed position.
out vec3 untrans_position;
// Position after model and view transformation.
out vec3 mv_position;
// Normalized normal after model and view transformation.
out vec3 unit_normal;
// 2D Texture coordinate.
out vec2 uv;

void main() {
    untrans_position = position;
    mv_position = vec3(mv * vec4(position, 1.0));
    unit_normal = normalize(vec3(inverse(transpose(mv)) * vec4(normal, 0.0)));
    // Using repeat texture wrap.
    uv = position.xz;
    gl_Position = mvp * vec4(position, 1.0);
}
