#version 330 core
out vec4 color;

in vec3 v_fragPos;
in vec3 v_normal;

uniform vec3 u_lightPos;
uniform vec3 u_lightColor;
uniform vec3 u_objectColor;

void main() {
    float ambientStrength = 0.1f;
    vec3 ambientLight = ambientStrength * u_lightColor;

    vec3 normal = normalize(v_normal);
    vec3 lightDirection = normalize(u_lightPos - v_fragPos);
    vec3 diffuseLight = max(dot(normal, lightDirection), 0.0) * u_lightColor;

    vec3 resLight = ambientLight + diffuseLight;
    color = vec4(resLight * u_objectColor, 1.0f);
}