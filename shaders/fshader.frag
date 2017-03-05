#version 330 core

in vec3 colour;
in vec2 tex_coord;

out vec4 col;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float opacity;

void main()
{
	col = mix(texture(tex1, tex_coord), texture(tex2, tex_coord), opacity);
}