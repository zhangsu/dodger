#version 330

uniform mat4 mv;
uniform mat4 mvp;
uniform mat4 shadow_mvp;

layout(location = 0) in vec3 position;
in vec3 normal;
// Untransformed position.
out vec3 untrans_position;
// Position after model and view transformation.
out vec3 mv_position;
// Normalized normal after model and view transformation.
out vec3 unit_normal;
// 2D Texture coordinate.
out vec2 uv;
// Shadow map coordinate.
out vec4 shadow_coord;

void main() {
    untrans_position = position;
    mv_position = vec3(mv * vec4(position, 1.0));
    unit_normal = normalize(vec3(inverse(transpose(mv)) * vec4(normal, 0.0)));
    // Using repeat texture wrap.
    uv = position.xz;
    shadow_coord = shadow_mvp * vec4(position, 1.0);
    gl_Position = mvp * vec4(position, 1.0);
}
