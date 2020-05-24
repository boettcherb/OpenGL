// VERTEX

#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_texCoord;

out vec2 v_texCoord;

uniform mat4 u_trans;

void main() {
    gl_Position = u_trans * vec4(a_position, 1.0f);
    v_texCoord = a_texCoord;
}