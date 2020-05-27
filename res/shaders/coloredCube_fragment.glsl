#version 330 core
out vec4 color;

uniform vec3 u_lightColor;
uniform vec3 u_objectColor;

void main() {
    float ambientStrength = 0.1f;
    vec3 ambientLight = ambientStrength * u_lightColor;
    color = vec4(ambientLight * u_objectColor, 1.0f);
}