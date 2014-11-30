#version 330

uniform mat4 mvp;

in vec3 position;
in vec4 metric;
in vec4 color;
out vec4 frag_color;

void main() {
    vec3 pos = position * metric.w + metric.xyz;
    gl_Position = mvp * vec4(pos, 1.0);
    frag_color = color;
}
