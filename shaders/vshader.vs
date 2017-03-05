#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colour;

uniform float offset;

out vec4 pos;

void main()
{
	gl_Position = vec4(-position.x + offset, -position.y + offset, -position.z, 1.0f);
	pos = gl_Position;
}
