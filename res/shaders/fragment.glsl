// FRAGMENT

#version 330 core
out vec4 color;

in vec4 v_color;
in vec2 v_texCoord;

uniform sampler2D u_texture_face;
uniform sampler2D u_texture_shelf;

void main() {
    vec4 face = texture(u_texture_face, v_texCoord);
    vec4 shelf = texture(u_texture_shelf, v_texCoord);
    color = mix(shelf, face, 0.2f) * v_color;
}