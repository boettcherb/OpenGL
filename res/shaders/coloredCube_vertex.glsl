#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;

out vec3 v_fragPos;
out vec3 v_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
    v_fragPos = vec3(u_model * vec4(a_position, 1.0f));
    v_normal = mat3(transpose(inverse(u_model))) * a_normal;
}