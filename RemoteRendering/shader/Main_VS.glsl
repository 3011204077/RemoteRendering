#version 150 core

uniform mat4 model;
uniform mat4 viewProj;

in vec3 vs_in_pos;
in vec3 vs_in_color;
in vec2 vs_in_texCoords;

out vec3 fs_in_color;
out vec2 fs_in_texCoords;

void main(void) {
    gl_Position = viewProj * vec4(vs_in_pos, 1);
	fs_in_texCoords = vs_in_texCoords;
}