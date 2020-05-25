// FRAGMENT

#version 330 core
out vec4 color;

in vec2 v_texCoord;

uniform sampler2D u_texture_shelf;
uniform sampler2D u_texture_face;

void main() {
    vec4 shelf = texture(u_texture_shelf, v_texCoord);
    vec4 face = texture(u_texture_face, v_texCoord);
    color = mix(shelf, face, 0.2f);
}