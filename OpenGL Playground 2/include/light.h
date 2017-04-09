#ifndef light_h
#define light_h

#include <cstdio>

#include <GLEW\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "shader.h"

struct Light
{
	Shader* shader;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Light(void) {}
	Light(Shader* shader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : shader(shader), ambient(ambient), diffuse(diffuse), specular(specular) {}
	virtual ~Light(void) {}
};

struct PointLight : public Light
{
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;

	PointLight(void) {}
	PointLight(Shader* shader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, int index, glm::vec3 position, float constant, float linear, float quadratic) 
		: Light(shader, ambient, diffuse, specular), position(position), constant(constant), linear(linear), quadratic(quadratic)
	{
		char buf[50];
		sprintf(buf, "point_lights[%d]", index);

		std::string light(buf);

		glUniform3f(glGetUniformLocation(shader->program, (light + ".position").c_str()), position.x, position.y, position.z);

		glUniform3f(glGetUniformLocation(shader->program, (light + ".ambient").c_str()), ambient.x, ambient.y, ambient.z);
		glUniform3f(glGetUniformLocation(shader->program, (light + ".diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(glGetUniformLocation(shader->program, (light + ".specular").c_str()), specular.x, specular.y, specular.z);

		glUniform1f(glGetUniformLocation(shader->program, (light + ".constant").c_str()), constant);
		glUniform1f(glGetUniformLocation(shader->program, (light + ".linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(shader->program, (light + ".quadratic").c_str()), quadratic);
	}
};

struct DirectionalLight : public Light
{
	glm::vec3 direction;

	DirectionalLight(Shader* shader, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		: Light(shader, ambient, diffuse, specular)
	{
		glUniform3f(glGetUniformLocation(shader->program, "directional_light.direction"), direction.x, direction.y, direction.z);

		glUniform3f(glGetUniformLocation(shader->program, "directional_light.ambient"), ambient.x, ambient.y, ambient.z);
		glUniform3f(glGetUniformLocation(shader->program, "directional_light.diffuse"), diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(glGetUniformLocation(shader->program, "directional_light.specular"), specular.x, specular.y, specular.z);
	}
};

struct SpotLight : public Light
{
	glm::vec3 position;
	glm::vec3 direction;
	float outer_cutoff;
	float inner_cutoff;
	float constant;
	float linear;
	float quadratic;

	SpotLight(Shader* shader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, float outer_cutoff, float inner_cutoff, float constant, float linear, float quadratic)
		: Light(shader, ambient, diffuse, specular), position(position), direction(direction), outer_cutoff(outer_cutoff), inner_cutoff(inner_cutoff), constant(constant), linear(linear), quadratic(quadratic)
	{
		glUniform3f(glGetUniformLocation(shader->program, "spot_light.position"), position.x, position.y, position.z);
		glUniform3f(glGetUniformLocation(shader->program, "spot_light.direction"), direction.x, direction.y, direction.z);

		glUniform3f(glGetUniformLocation(shader->program, "spot_light.ambient"), ambient.x, ambient.y, ambient.z);
		glUniform3f(glGetUniformLocation(shader->program, "spot_light.diffuse"), diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(glGetUniformLocation(shader->program, "spot_light.specular"), specular.x, specular.y, specular.z);

		glUniform1f(glGetUniformLocation(shader->program, "spot_light.inner_cutoff"), inner_cutoff);
		glUniform1f(glGetUniformLocation(shader->program, "spot_light.outer_cutoff"), outer_cutoff);

		glUniform1f(glGetUniformLocation(shader->program, "spot_light.constant"), constant);
		glUniform1f(glGetUniformLocation(shader->program, "spot_light.linear"), linear);
		glUniform1f(glGetUniformLocation(shader->program, "spot_light.quadratic"), quadratic);
	}

	void update(glm::vec3 position, glm::vec3 direction)
	{
		this->position = position;
		this->direction = direction;

		glUniform3f(glGetUniformLocation(shader->program, "spot_light.position"), position.x, position.y, position.z);
		glUniform3f(glGetUniformLocation(shader->program, "spot_light.direction"), direction.x, direction.y, direction.z);
	}
};

#endif