#version 330

uniform mat4 mvp;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 metric;
layout(location = 2) in vec4 color;

void main() {
    vec3 pos = position * metric.w + metric.xyz;
    gl_Position = mvp * vec4(pos, 1.0);
}
