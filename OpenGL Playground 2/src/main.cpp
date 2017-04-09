#define GLEW_STATIC
#define _CRT_SECURE_NO_WARNINGS

#include <GLEW\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL\SOIL.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "light.h"

GLint WIDTH = 800;
GLint HEIGHT = 600;

GLfloat vertices[] = {
	// Positions          Normals              UVs
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 cube_positions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.4f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f),
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Bind arrays for lamp cube. Array buffer already bound
	glBindVertexArray(lamp_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int width, height;
	GLuint diffuse_map, specular_map, emission_map;
	glGenTextures(1, &diffuse_map);
	glGenTextures(1, &specular_map);
	//glGenTextures(1, &emission_map);

	unsigned char* image_data = SOIL_load_image("../textures/container2.png", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuse_map);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image_data);
	glBindTexture(GL_TEXTURE_2D, 0);

	image_data = SOIL_load_image("../textures/container2_specular.png", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specular_map);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image_data);
	glBindTexture(GL_TEXTURE_2D, 0);

	/*image_data = SOIL_load_image("../textures/matrix.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, emission_map);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image_data);
	glBindTexture(GL_TEXTURE_2D, 0);*/
	
	shader_program.use();

	// Material colours
	glUniform1f(glGetUniformLocation(shader_program.program, "material.shininess"), 32.0f);
	glUniform1i(glGetUniformLocation(shader_program.program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(shader_program.program, "material.specular"), 1);

	SpotLight slight(&shader_program, glm::vec3(.05f, .05f, .05f), glm::vec3(.8f, .8f, .8f), glm::vec3(1.f, 1.f, 1.f), camera.get_position(), camera.get_front(), glm::cos(glm::radians(17.5f)), glm::cos(glm::radians(15.5f)), 1.f, .09f, .032f);
	DirectionalLight dlight(&shader_program, glm::vec3(-.2f, -1.f, -.3f), glm::vec3(.05f, .05f, .05f), glm::vec3(.4f, .4f, .4f), glm::vec3(.5f, .5f, .5f));
	PointLight point_lights[4] = {
		PointLight(&shader_program, glm::vec3(.05f, .05f, .05f), glm::vec3(.8f, .8f, .8f), glm::vec3(1.f, 1.f, 1.f), 0, glm::vec3(.7f, .2f, 2.f), 1.f, .09f, .032f),
		PointLight(&shader_program, glm::vec3(.05f, .05f, .05f), glm::vec3(.8f, .8f, .8f), glm::vec3(1.f, 1.f, 1.f), 1, glm::vec3(2.3f, -3.3f, -4.f), 1.f, .09f, .032f),
		PointLight(&shader_program, glm::vec3(.05f, .05f, .05f), glm::vec3(.8f, .8f, .8f), glm::vec3(1.f, 1.f, 1.f), 2, glm::vec3(-4.f, 2.f, -12.f), 1.f, .09f, .032f),
		PointLight(&shader_program, glm::vec3(.05f, .05f, .05f), glm::vec3(.8f, .8f, .8f), glm::vec3(1.f, 1.f, 1.f), 3, glm::vec3(0.f, 0.f, -3.f), 1.f, .09f, .032f)
	};

	while (!glfwWindowShouldClose(window))
	{
		GLfloat current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		glfwPollEvents();
		move();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader_program.use();

		glm::vec3 view_position = camera.get_position();
		glm::vec3 camera_front = camera.get_front();

		slight.update(view_position, camera_front);
		glUniform3f(glGetUniformLocation(shader_program.program, "view_position"), view_position.x, view_position.y, view_position.z);

		glm::mat4 model, view, projection;
		view = camera.get_view_matrix();
		projection = glm::perspective(camera.get_zoom(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader_program.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader_program.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse_map);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular_map);

		// Draw the boxes
		glBindVertexArray(cube_vao);
		for (GLuint i = 0; i < sizeof(cube_positions) / sizeof(glm::vec3); i++)
		{
			model = glm::mat4();
			model = glm::translate(model, cube_positions[i]);
			model = glm::rotate(model, glm::radians((GLfloat)i * 20.f), glm::vec3(1.0f, 0.3f, 0.5f));

			glUniformMatrix4fv(glGetUniformLocation(shader_program.program, "model"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		lamp_shader.use();

		glUniformMatrix4fv(glGetUniformLocation(lamp_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lamp_shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Draw spinning cubes to represent the point lights
		glBindVertexArray(lamp_vao);
		for (int i = 0; i < sizeof(point_lights) / sizeof(PointLight); i++)
		{
			model = glm::mat4();
			model = glm::translate(model, point_lights[i].position);
			model = glm::rotate(model, glm::radians(180.0f * (GLfloat)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.2f));

			glUniformMatrix4fv(glGetUniformLocation(lamp_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);	
		}
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
}