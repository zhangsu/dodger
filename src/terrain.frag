#version 330

struct Light {
    vec3 position;
    vec3 color;
    vec3 attenuation;
};

uniform sampler2D grass_sampler;
uniform sampler2D rock_sampler;
uniform sampler2D shadowmap_sampler;

uniform Light lights[16];
uniform int light_count;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

in vec3 untrans_position;
in vec3 mv_position;
in vec3 unit_normal;
in vec2 uv;
out vec4 frag_color;

void main() {
    // Texture mapping.
    float altitude = untrans_position.y;
    vec3 tex_color = mix(texture(grass_sampler, uv).rgb,
                         texture(rock_sampler, uv).rgb, altitude);

    // Phong Lighting.
    vec3 color = tex_color * ambient;
    for (int i = 0; i < light_count; ++i) {
        vec3 position2light = lights[i].position - mv_position;
        float dist = length(position2light);
        vec3 light_direction = normalize(position2light);
        vec3 eye_pos = normalize(-mv_position);
        vec3 r = normalize(-reflect(light_direction, unit_normal));

        vec3 intensity =
            1.0 / (lights[i].attenuation.x
                   + lights[i].attenuation.y * dist
                   + lights[i].attenuation.z * dist * dist)
            * lights[i].color;

        vec3 diffuse_color = intensity * diffuse
            * max(dot(unit_normal, light_direction), 0.0);
        vec3 specular_color = intensity * specular
            * pow(max(dot(r, eye_pos), 0.0), shininess);
        diffuse_color = clamp(diffuse_color, 0.0, 1.0);
        specular_color = clamp(specular_color, 0.0, 1.0);

        color += tex_color * diffuse_color + specular_color;
    }
    frag_color = vec4(color, 1.0);
}
