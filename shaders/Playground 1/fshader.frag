#version 330 core

in vec2 tex_coord;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
	gl_FragColor = mix(texture(tex1, tex_coord), texture(tex2, tex_coord), 0.2);
}