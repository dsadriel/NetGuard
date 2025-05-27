#ifndef _CAMERA_H
#define _CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "matrices.h"

class Camera {
  private:
  public:
	glm::vec4 position;
	glm::vec4 up_vector;
	float yaw;
	float pitch;
	glm::vec4 view_vector;

	Camera(glm::vec4 pos, float yaw_rad, float pitch_rad)
	    : position(pos), up_vector(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)), yaw(yaw_rad), pitch(pitch_rad) {
		updateViewVector();
	}

	glm::mat4 getViewMatrix() { return Matrix_Camera_View(position, view_vector, up_vector); }

	void updateViewVector() {
		view_vector = normalize(glm::vec4(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw), 0.0f));
	}
};

#endif // CAMERA_H
