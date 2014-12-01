#version 330

struct Light {
    vec3 position;
    vec3 color;
    vec3 attenuation;
    bool cast_shadow;
};

uniform bool drawing_shadow;

uniform sampler2D grass_sampler;
uniform sampler2D rock_sampler;
uniform sampler2DShadow shadowmap_sampler;

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
in vec4 shadow_coord;
out vec4 frag_color;

vec2 poisson_consts[16] = vec2[](
   vec2(-0.94201624, -0.39906216),
   vec2(0.94558609, -0.76890725),
   vec2(-0.094184101, -0.92938870),
   vec2(0.34495938, 0.29387760),
   vec2(-0.91588581, 0.45771432),
   vec2(-0.81544232, -0.87912464),
   vec2(-0.38277543, 0.27676845),
   vec2(0.97484398, 0.75648379),
   vec2(0.44323325, -0.97511554),
   vec2(0.53742981, -0.47373420),
   vec2(-0.26496911, -0.41893023),
   vec2(0.79197514, 0.19090188),
   vec2(-0.24188840, 0.99706507),
   vec2(-0.81409955, 0.91437590),
   vec2(0.19984126, 0.78641367),
   vec2(0.14383161, -0.14100790)
);

float computeShades() {
    float shades = 1.0;
    float bias = 0.0001;
    // Poisson disc multisampling on the shadow map.
    for (int i = 0; i < 4; i++) {
        shades -=
            (1.0 - texture(
                shadowmap_sampler,
                vec3(
                    shadow_coord.xy + poisson_consts[i] / 700.0,
                    (shadow_coord.z - bias) / shadow_coord.w
                )
            )) * 0.2;
    }
    return shades;
}

void main() {
    // Texture mapping.
    float altitude = untrans_position.y;

    vec3 tex_color = mix(texture(grass_sampler, uv).rgb,
                         texture(rock_sampler, uv).rgb, altitude);
    float shades = 1.0;
    if (drawing_shadow)
        shades = computeShades();

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

        vec3 light_color = tex_color * diffuse_color + specular_color;
        if (lights[i].cast_shadow)
            light_color *= shades;
        color += light_color;
    }
    frag_color = vec4(color, 1.0);
}
