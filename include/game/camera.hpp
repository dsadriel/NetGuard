#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "matrices.hpp"

enum class CameraMode { FirstPerson, LookAt, TopDown };
enum class CameraMovement { Forward, Backward, Left, Right, Up, Down };

class Camera {
  private:
	glm::vec4 view_vector;
	float yaw;
	float pitch;

  public:
	glm::vec4 position;
	glm::vec4 up_vector;
	glm::vec4 target = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	float nearplane = -0.1f;
	float farplane = -10.0f;
	double field_of_view = 45.0;

	 Camera(glm::vec4 pos, float yaw_rad, float pitch_rad)
        : yaw(yaw_rad), pitch(pitch_rad), position(pos), up_vector(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)) {
        updateViewVector();
    }
	
	glm::mat4 getViewMatrix() { return Matrix_Camera_View(position, view_vector, up_vector); }
	glm::mat4 getProjectionMatrix(double screenRatio) {
		return Matrix_Perspective(glm::radians(field_of_view), screenRatio, nearplane, farplane);
	}

	void updateViewVector() {
		view_vector = normalize(glm::vec4(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw), 0.0f));
	}

	// MARK: Camera Movement
	void move(CameraMovement direction, float speed) {
		glm::vec4 right = normalize(crossproduct(view_vector, up_vector));
		switch (direction) {
		case CameraMovement::Forward:
			position += view_vector * speed;
			break;
		case CameraMovement::Backward:
			position -= view_vector * speed;
			break;
		case CameraMovement::Left:
			position -= right * speed;
			break;
		case CameraMovement::Right:
			position += right * speed;
			break;
		case CameraMovement::Up:
			position += up_vector * speed;
			break;
		case CameraMovement::Down:
			position -= up_vector * speed;
			break;
		}
	}

	void changeYaw(float delta_yaw) {
		yaw += delta_yaw;
		updateViewVector();
	}

	void changePitch(float delta_pitch) {
		pitch += delta_pitch;
		if (pitch > glm::radians(89.0f))
			pitch = glm::radians(89.0f);
		if (pitch < glm::radians(-89.0f))
			pitch = glm::radians(-89.0f);
		updateViewVector();
	}
};

#endif // CAMERA_H
