#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include <external/glm/glm.hpp>
#include <external/glm/mat4x4.hpp>
#include <external/glm/vec4.hpp>

#include "matrices.hpp"
#include "collisions.hpp"

using namespace glm;

enum class CameraMode { Free, LookAt, TopDown };
enum class CameraMovement { Forward, Backward, Left, Right, Up, Down };

class Camera {
  private:
	vec4 view_vector;
	float yaw;
	float pitch;

  public:
	vec4 position;
	vec4 up_vector;
	vec4 target = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	CameraMode mode = CameraMode::Free;

	float nearplane = -0.1f;
	float farplane = -100.0f;
	double field_of_view = 45.0;

	Camera(vec4 pos, float yaw_rad, float pitch_rad)
	    : yaw(yaw_rad), pitch(pitch_rad), position(pos), up_vector(vec4(0.0f, 1.0f, 0.0f, 0.0f)) {
		updateViewVector();
	}

	mat4 getViewMatrix() {
		vec4 view_vector_;
		vec4 up_vec_;

		switch (mode) {
		case CameraMode::LookAt:
			view_vector_ = target - position;
			up_vec_ = up_vector;
			return Matrix_Camera_View(position, view_vector_, up_vec_);

		case CameraMode::TopDown:
			view_vector_ = vec4(0.0f, -1.0f, 0.0f, 0.0f);
			up_vec_ = vec4(0.0f, 0.0f, 1.0f, 0.0f);
			return Matrix_Camera_View(position, view_vector_, up_vec_);

		default: // CameraMode::Free ou outras
			view_vector_ = view_vector;
			up_vec_ = up_vector;
			return Matrix_Camera_View(position, view_vector_, up_vec_);
		}
	}

	mat4 getProjectionMatrix(double screenRatio) {
		return Matrix_Perspective(radians(field_of_view), screenRatio, nearplane, farplane);
	}

	void updateViewVector() {
		view_vector = normalize(vec4(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw), 0.0f));
	}

	// MARK: Camera Movement
	void move(CameraMovement direction, float speed) {
		vec4 right = normalize(crossproduct(view_vector, up_vector));
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
		if (pitch > radians(89.0f))
			pitch = radians(89.0f);
		if (pitch < radians(-89.0f))
			pitch = radians(-89.0f);
		updateViewVector();
	}

	Ray getPickingRay(float screen_width, float screen_height, float cursor_pos_x, float cursor_pos_y) {
		float x_ndc = (2.0f * cursor_pos_x) / screen_width - 1.0f;
		float y_ndc = 1.0f - (2.0f * cursor_pos_y) / screen_height;
		float z_ndc = this->nearplane;

		vec4 ray_ndc(x_ndc, y_ndc, z_ndc, 1.0f);

		// Convert NDC to camera coordinates
		mat4 projection_matrix = getProjectionMatrix(screen_width / screen_height);
		vec4 ray_camera = inverse(projection_matrix) * ray_ndc;
		vec4 ray_dir_camera = vec4(ray_camera.x, ray_camera.y, -1.0f, 0.0f);

		// Convert camera coordinates to world coordinates
		mat4 view_matrix = getViewMatrix();
		vec4 ray_dir_world = inverse(view_matrix) * ray_dir_camera;
		ray_dir_world = normalize(ray_dir_world);

		return Ray(this->position, ray_dir_world);
	}
	
};

#endif // CAMERA_H
