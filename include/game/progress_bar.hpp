#if !defined(PROGRESS_BAR_HPP)
#define PROGRESS_BAR_HPP

#include "camera.hpp"
#include "scene.hpp"
#include <external/glm/vec4.hpp>

class ProgressBar {
  public:
	vec3 progressColor;       // Color of the progress bar
	vec3 backgroundColor;     // Background color of the progress bar
	bool alwaysFaceCamera;    // If true, the progress bar will always face the camera
	float progress = 0.0f;    // Progress value (0.0 to 1.0)
	SceneObject *planeObject; // Scene object representing the progress bar
	vec4 position;            // Position of the unit

	ProgressBar()
	    : position(vec4(0.0f)), progressColor(vec3(0.0f, 1.0f, 0.0f)), backgroundColor(vec3(0.2f, 0.2f, 0.2f)),
	      alwaysFaceCamera(true), planeObject(nullptr) {}

	ProgressBar(vec4 pos, vec3 progColor, vec4 bgColor, bool faceCamera = true)
	    : position(pos), progressColor(progColor), backgroundColor(bgColor), alwaysFaceCamera(faceCamera),
	      planeObject(nullptr) {}
	ProgressBar(vec4 pos, vec3 progColor, vec3 bgColor, SceneObject *planeObj, bool faceCamera = true)
	    : position(pos), progressColor(progColor), backgroundColor(bgColor), alwaysFaceCamera(faceCamera),
	      planeObject(planeObj) {}

	void draw(Camera camera, GLint model_uniform, GLint object_style_uniform, GLint object_color_uniform,
	          GLint shading_mode_uniform) {
		static float barWidth = 0.5f;
		static float barHeight = 0.05f;
		static float insetPadding = 0.01f; // Padding around the progress bar

		planeObject->rotation = calculateRotation(camera);
		planeObject->scale = vec3(0.0f, 1.0f, barHeight);
		planeObject->object_style = PLAIN_COLOR;
		planeObject->shading_mode = SHADING_GOURAUD;

		if (planeObject) {
			// Draw the background
			planeObject->color = vec4(backgroundColor, 1.0f);

			planeObject->position = position;
			planeObject->scale.x = barWidth; // Set width of the background

			planeObject->drawObject(model_uniform, object_style_uniform, object_color_uniform, shading_mode_uniform);

			// Draw the progress bar with padding
			planeObject->color = vec4(progressColor, 1.0f);

			vec4 relativePosition = position;

			float paddedBarWidth = barWidth - 2.0f * insetPadding;
			float paddedBarHeight = barHeight - 2.0f * insetPadding;

			relativePosition.x -= barWidth / 2.0f; // Start at the left edge of the background
			relativePosition.x += insetPadding + (paddedBarWidth * progress) /
			                                         2.0f; // Move center as progress grows, with absolute padding
			relativePosition.y += 0.00001f;                // Slightly above the background to avoid z-fighting

			planeObject->position = relativePosition;

			planeObject->scale.x = paddedBarWidth * progress;
			planeObject->scale.z = paddedBarHeight;

			planeObject->drawObject(model_uniform, object_style_uniform, object_color_uniform, shading_mode_uniform);
		}
	}

  private:
	vec4 calculateRotation(const Camera &camera) {
		if (alwaysFaceCamera) {
			// MARK: TODO: Implement proper rotation calculation
			return vec4(0.0f, 0.0f, 0.0f, 0.0f);
		} else {
			return vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}
};

#endif // PROGRESS_BAR_HPP
