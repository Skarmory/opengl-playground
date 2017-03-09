#ifndef  shader_h
#define shader_h

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GLEW/glew.h>

class Shader
{
public:
	GLuint program;

	Shader(const GLchar* vertex_path, const GLchar* fragment_path)
	{
		// Read vertex and fragment shaders
		std::string vertex_code, fragment_code;
		std::ifstream vertex_shader_file, fragment_shader_file;

		vertex_shader_file.exceptions(std::ifstream::badbit);
		fragment_shader_file.exceptions(std::ifstream::badbit);

		try
		{
			vertex_shader_file.open(vertex_path);
			fragment_shader_file.open(fragment_path);

			std::stringstream vertex_shader_stream, fragment_shader_stream;

			vertex_shader_stream << vertex_shader_file.rdbuf();
			fragment_shader_stream << fragment_shader_file.rdbuf();

			vertex_shader_file.close();
			fragment_shader_file.close();

			vertex_code = vertex_shader_stream.str();
			fragment_code = fragment_shader_stream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "Error: shader file was not read correctly" << std::endl;
		}

		const GLchar* vertex_shader_code = vertex_code.c_str();
		const GLchar* fragment_shader_code = fragment_code.c_str();

		// Compile shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar log[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertex_shader_code, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, log);
			std::cout << "Error: vertex shader failed to compile\n" << log << std::endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragment_shader_code, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, log);
			std::cout << "Error: fragment shader failed to compile\n" << log << std::endl;
		}

		// Attach shaders to program and link
		this->program = glCreateProgram();
		glAttachShader(this->program, vertex);
		glAttachShader(this->program, fragment);
		glLinkProgram(this->program);

		glGetProgramiv(this->program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->program, 512, NULL, log);
			std::cout << "Error: shader program linking failed\n" << log << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void use(void)
	{
		glUseProgram(this->program);
	}
};

#endif // ! shader_h
