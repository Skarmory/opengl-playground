#version 330 core

uniform vec3 object_colour;
uniform vec3 light_colour;

void main()
{
	gl_FragColor = vec4(light_colour * object_colour, 1);
}