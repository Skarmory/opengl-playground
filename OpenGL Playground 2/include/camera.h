#ifndef camera_h
#define camera_h

#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

const GLfloat YAW         = -90.0f;
const GLfloat PITCH       = 0.0f;
const GLfloat SPEED       = 3.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM        = 45.0f;

class Camera
{
public:

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) 
		: front(glm::vec3(0.0f, 0.0f, -1.0f)), move_speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM)
	{
		this->position = position;
		this->world_up = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->update();
	}

	Camera(GLfloat x, GLfloat y, GLfloat z, GLfloat up_x, GLfloat up_y, GLfloat up_z, GLfloat yaw, GLfloat pitch)
		: front(glm::vec3(0.0f, 0.0f, -1.0f)), move_speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM)
	{
		this->position = glm::vec3(x, y, z);
		this->world_up = glm::vec3(up_x, up_y, up_z);
		this->yaw = yaw;
		this->pitch = pitch;
		this->update();
	}

	glm::mat4 get_view_matrix(void)
	{
		return glm::lookAt(position, position + front, up);
	}

	GLfloat get_zoom(void)
	{
		return zoom;
	}

	glm::vec3 get_front(void)
	{
		return front;
	}

	glm::vec3 get_position(void)
	{
		return position;
	}

	void process_keyboard(CameraMovement direction, GLfloat delta_time)
	{
		GLfloat velocity = move_speed * delta_time;
		
		if(direction == FORWARD)
			position += front * velocity;
		if(direction == BACKWARD)
			position -= front * velocity;
		if(direction == LEFT)
			position -= right * velocity;
		if(direction == RIGHT)
			position += right * velocity;
		if (direction == UP)
			position += up * velocity;
		if (direction == DOWN)
			position -= up * velocity;
	}

	void process_mouse(GLfloat x_offset, GLfloat y_offset, GLboolean constrain_pitch = true)
	{
		x_offset *= sensitivity;
		y_offset *= sensitivity;

		yaw   += x_offset;
		pitch += y_offset;

		if (constrain_pitch)
		{
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}

		update();
	}

	void process_scroll(GLfloat y_offset)
	{
		if (zoom >= 1.0f && zoom <= 45.0f)
			zoom -= y_offset;
		if (zoom <= 1.0f)
			zoom = 1.0f;
		if(zoom >= 45.0f)
			zoom = 45.0f;
	}

private:
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 world_up;

	GLfloat pitch;
	GLfloat yaw;

	GLfloat move_speed;
	GLfloat sensitivity;
	GLfloat zoom;

	void update(void)
	{
		glm::vec3 f;
		f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		f.y = sin(glm::radians(pitch));
		f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		front = glm::normalize(f);
		right = glm::normalize(glm::cross(front, world_up));
		up    = glm::normalize(glm::cross(right, front));
	}
};

#endif
