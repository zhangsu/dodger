#version 330

uniform sampler2D sampler;
uniform sampler2D mountain_sampler;

in vec3 untrans_position;
in vec3 unit_normal;
in vec2 uv;
out vec4 color;

void main() {
    float altitude = untrans_position.y;
    color = 0.001*vec4(unit_normal, 0)
            + texture(sampler, uv) * (1 - altitude)
            + texture(mountain_sampler, uv) * altitude;
}
