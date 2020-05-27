#version 330 core
out vec4 color;

uniform vec3 u_lightColor;
uniform vec3 u_objectColor;

void main() {
    color = vec4(u_lightColor * u_objectColor, 1.0f);
}