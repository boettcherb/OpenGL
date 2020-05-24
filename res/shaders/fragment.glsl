// FRAGMENT

#version 330 core
out vec4 color;

in vec2 v_texCoord;

uniform float u_offsetX;
uniform float u_offsetY;
uniform sampler2D u_texture_gradient;

void main() {
    vec2 final = vec2(v_texCoord.x + u_offsetX, v_texCoord.y + u_offsetY);
    color = texture(u_texture_gradient, final);
}