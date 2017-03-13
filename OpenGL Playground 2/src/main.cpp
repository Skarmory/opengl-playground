#define GLEW_STATIC

#include <GLEW\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL\SOIL.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "shader.h"
#include "camera.h"

GLint WIDTH = 800;
GLint HEIGHT = 600;

GLfloat vertices[] = {
	// Positions          // Normals
	-0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,

	-0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
};

bool keys[1024];
bool first_mouse_input = true;

GLfloat delta_time = 0.0f;
GLfloat last_frame = 0.0f;

GLfloat last_x = WIDTH / 2.0f;
GLfloat last_y = HEIGHT / 2.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void move()
{
	if (keys[GLFW_KEY_W])
		camera.process_keyboard(FORWARD, delta_time);
	if (keys[GLFW_KEY_S])
		camera.process_keyboard(BACKWARD, delta_time);
	if (keys[GLFW_KEY_A])
		camera.process_keyboard(LEFT, delta_time);
	if (keys[GLFW_KEY_D])
		camera.process_keyboard(RIGHT, delta_time);
	if (keys[GLFW_KEY_SPACE])
		camera.process_keyboard(UP, delta_time);
	if (keys[GLFW_KEY_LEFT_CONTROL])
		camera.process_keyboard(DOWN, delta_time);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (first_mouse_input)
	{
		last_x = xpos;
		last_y = ypos;
		first_mouse_input = false;
	}

	GLfloat xoffset = xpos - last_x;
	GLfloat yoffset = last_y - ypos;
	last_x = xpos;
	last_y = ypos;

	camera.process_mouse(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.process_scroll(yoffset);
}

void main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Playground 2 - Lighting and Colours", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);

	const GLchar* vpath = "../shaders/Playground 2/vshader.vs";
	const GLchar* fpath = "../shaders/Playground 2/fshader.frag";
	const GLchar* lamp_vpath = "../shaders/Playground 2/lamp_vshader.vs";
	const GLchar* lamp_fpath = "../shaders/Playground 2/lamp_fshader.frag";

	Shader shader_program(vpath, fpath);
	Shader lamp_shader(lamp_vpath, lamp_fpath);

	GLuint cube_vao, lamp_vao, vbo;
	glGenVertexArrays(1, &cube_vao);
	glGenVertexArrays(1, &lamp_vao);
	glGenBuffers(1, &vbo);

	// Bind arrays and buffers for cube
	glBindVertexArray(cube_vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Bind arrays for lamp cube, array buffer already bound
	glBindVertexArray(lamp_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glm::vec3 light_position(1.2f, 0.5f, 2.0f);
	glm::vec3 cube_position(0.0f, 0.0f, 0.0f);
	glm::vec3 light_colour;

	while (!glfwWindowShouldClose(window))
	{
		GLfloat current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		glfwPollEvents();
		move();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader_program.use();

		light_colour.x = sin(glfwGetTime() * 2.0f);
		light_colour.y = sin(glfwGetTime() * 0.75f);
		light_colour.z = sin(glfwGetTime() * 1.5f);

		glm::vec3 diffuse = light_colour * glm::vec3(0.5f);
		glm::vec3 ambient = light_colour * glm::vec3(0.2f);

		glm::mat4 model, view, projection;
		view = camera.get_view_matrix();
		projection = glm::perspective(camera.get_zoom(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f);

		glm::vec3 view_position = camera.get_position();

		// Material colours
		GLint mat_ambient_loc = glGetUniformLocation(shader_program.program, "material.ambient");
		GLint mat_diffuse_loc = glGetUniformLocation(shader_program.program, "material.diffuse");
		GLint mat_specular_loc = glGetUniformLocation(shader_program.program, "material.specular");
		GLint mat_shininess_loc = glGetUniformLocation(shader_program.program, "material.shininess");
		
		glUniform3f(mat_ambient_loc, 1.0f, 0.5f, 0.3f);
		glUniform3f(mat_diffuse_loc, 1.0f, 0.5f, 0.3f);
		glUniform3f(mat_specular_loc, 0.5f, 0.5f, 0.5f);
		glUniform1f(mat_shininess_loc, 32.0f);

		// Light properties
		GLint light_position_loc = glGetUniformLocation(shader_program.program, "light_position");
		GLint light_ambient_loc = glGetUniformLocation(shader_program.program, "light.ambient");
		GLint light_diffuse_loc = glGetUniformLocation(shader_program.program, "light.diffuse");
		GLint light_specular_loc = glGetUniformLocation(shader_program.program, "light.specular");
		glUniform3f(light_position_loc, light_position.x, light_position.y, light_position.z);
		glUniform3f(light_ambient_loc, ambient.x, ambient.y, ambient.z);
		glUniform3f(light_diffuse_loc, diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(light_specular_loc, 1.0f, 1.0f, 1.0f);

		glUniformMatrix4fv(glGetUniformLocation(shader_program.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader_program.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader_program.program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(cube_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		lamp_shader.use();

		light_position.x = sin(glfwGetTime()) * 2.0f;
		light_position.z = cos(glfwGetTime()) * 2.0f;

		model = glm::mat4();
		model = glm::translate(model, light_position);
		model = glm::rotate(model, glm::radians(180.0f * (GLfloat)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));

		glUniformMatrix4fv(glGetUniformLocation(lamp_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lamp_shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(lamp_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(lamp_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
}