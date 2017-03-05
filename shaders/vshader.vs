#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 col;
layout (location = 2) in vec2 tex;

out vec3 colour;
out vec2 tex_coord;

void main()
{
	gl_Position = vec4(position, 1.0f);
	colour = col;
	tex_coord = vec2(tex.x, 1.0f - tex.y);
}
