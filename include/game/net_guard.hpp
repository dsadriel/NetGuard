#ifndef GAME_NET_GUARD_HPP
#define GAME_NET_GUARD_HPP

#include "collisions.hpp"
#include "game/camera.hpp"
#include "game/defense_unit.hpp"
#include "game/invasion_unit.hpp"
#include "scene.hpp"
#define GLFW_INCLUDE_NONE
#include <external/GLFW/glfw3.h>
#include <external/glm/vec4.hpp>
#include <string>
#include <vector>

using namespace glm;
using namespace std;

enum class NetGuardStage {
	onboarding,
	defenseDeployment,
	invasionPhase,
	invasionPhaseCompleted,
	gameOver,
	gameOverWithVictory,
	creditScreen
};

class NetGuard {
  private:
	NetGuardStage currentStage;

	// List of defense units
	vector<DefenseUnit> defenseUnits;
	// List of invasion units
	vector<InvasionUnit> invasionUnits;

	// Game state variables
	int currentInvasionWave = 0;
	int playerScore = 0;
	int playerLives = 3;

	float movementSpeed = 5.0f;

	int selectedUnitType = -1;

	// GLFW references
	GLFWwindow *window;
	GLint model_uniform;
	GLint object_style_uniform;
	GLint object_color_uniform;

	bool isLeftMouseButtonPressed = false;

	vec2 selectedPosition = vec2(-INFINITY, -INFINITY);
	
	const int gridHeight = 1.0f; 
  public:
	Camera camera = Camera(vec4(2.0f, 2.0f, 2.0f, 1.0f), -2.4f, -0.5f);

	// MARK: Initialization
	NetGuard() {
		currentStage = NetGuardStage::onboarding;
		defenseUnits.clear();
		invasionUnits.clear();
		currentInvasionWave = 0;
		playerScore = 0;
		playerLives = 3;
	}

	// Game SceneObjects
	SceneObject *map = nullptr;
	SceneObject *board = nullptr;
	SceneObject *cat = nullptr;
	SceneObject *plane = nullptr;
	SceneObject *antivirusSceneObject = nullptr;

	void link(GLFWwindow *window, GLint model_uniform, GLint object_style_uniform, GLint object_color_uniform) {
		this->window = window;
		this->model_uniform = model_uniform;
		this->object_style_uniform = object_style_uniform;
		this->object_color_uniform = object_color_uniform;
	}

	NetGuardStage getCurrentStage() const { return currentStage; }

	string getCurrentStageString() const {
		switch (currentStage) {
		case NetGuardStage::onboarding:
			return "onboarding";
		case NetGuardStage::defenseDeployment:
			return "defenseDeployment";
		case NetGuardStage::invasionPhase:
			return "invasionPhase";
		case NetGuardStage::invasionPhaseCompleted:
			return "invasionPhaseCompleted";
		case NetGuardStage::gameOver:
			return "gameOver";
		case NetGuardStage::gameOverWithVictory:
			return "gameOverWithVictory";
		case NetGuardStage::creditScreen:
			return "creditScreen";
		default:
			return "Unknown";
		}
	}

	void nexStage() {
		switch (currentStage) {
		case NetGuardStage::onboarding:
			currentStage = NetGuardStage::defenseDeployment;
			break;
		case NetGuardStage::defenseDeployment:
			currentStage = NetGuardStage::invasionPhase;
			break;
		case NetGuardStage::invasionPhase:
			currentStage = NetGuardStage::invasionPhaseCompleted;
			break;
		case NetGuardStage::invasionPhaseCompleted:
			currentStage = NetGuardStage::gameOverWithVictory; // or gameOver based on conditions
			break;
		case NetGuardStage::gameOver:
		case NetGuardStage::gameOverWithVictory:
			currentStage = NetGuardStage::creditScreen;
			break;
		case NetGuardStage::creditScreen:
			currentStage = NetGuardStage::onboarding; // Restart the game
			break;
		}
	}

	// MARK: Game loop

	void update(float deltaTime) {
		static int onboardingUpdateStage = 0;
		// Update game logic based on the current stage
		switch (currentStage) {
		case NetGuardStage::onboarding:
			onboardingUpdate(deltaTime, onboardingUpdateStage);
			break;
		case NetGuardStage::defenseDeployment:
			onboardingUpdateStage = 0; // Reset onboarding stage
			// Handle adding defense units logic
			defenseDeploymentUpdate();
			break;
		case NetGuardStage::invasionPhase:
			// Handle invasion phase logic
			camera.mode = CameraMode::Free;
			handleMovement(deltaTime);
			break;
		case NetGuardStage::invasionPhaseCompleted:
			// Handle invasion phase completed logic
			break;
		case NetGuardStage::gameOver:
			// Handle game over logic
			break;
		case NetGuardStage::gameOverWithVictory:
			// Handle game over with victory logic
			break;
		case NetGuardStage::creditScreen:
			// Handle credit screen logic
			break;
		}
	}

	// MARK: Draw
	void draw() {
		map->drawObject(model_uniform, object_style_uniform, object_color_uniform);
		board->drawObject(model_uniform, object_style_uniform, object_color_uniform);

		for (auto &defenseUnit : defenseUnits) {
			if (defenseUnit.sceneObject != nullptr) {
				defenseUnit.sceneObject->position = defenseUnit.getPosition();
				defenseUnit.sceneObject->drawObject(model_uniform, object_style_uniform, object_color_uniform);
			}
		}

		switch (currentStage) {
		case NetGuardStage::onboarding:
			// Draw onboarding screen
			break;
		case NetGuardStage::defenseDeployment:
			drawDefenseDeploymentScreen();
			break;
		case NetGuardStage::invasionPhase:
			// Draw invasion phase screen
			break;
		case NetGuardStage::invasionPhaseCompleted:
			// Draw invasion phase completed screen
			break;
		case NetGuardStage::gameOver:
			// Draw game over screen
			break;
		case NetGuardStage::gameOverWithVictory:
			// Draw victory screen
			break;
		case NetGuardStage::creditScreen:
			// Draw credit screen
			break;
		}
	}

	void defenseDeploymentUpdate() {
		camera.position = vec4(0.0f, 20.0f, 0.0f, 1.0f);
		camera.mode = CameraMode::TopDown;
		
		// Handle grid selection logic
		double cursorX, cursorY;
		glfwGetCursorPos(window, &cursorX, &cursorY);
		Ray pickingRay = camera.getPickingRay(1024.0f, 768.0f, cursorX, cursorY);

		// Check collision for each grid cell
		for (int x = -6; x <= 5; x++) {
			for (int z = -6; z <= 5; z++) {
				float centerX = x + 0.5f;
				float centerZ = z + 0.5f;
				
				float scaleX = 0.9f;
				float scaleZ = 0.9f;

				Plane planeAbs =
				    Plane(vec4(centerX - 0.5f * scaleX, 1.0f, centerZ - 0.5f * scaleZ, 1.0f),
				          vec4(centerX - 0.5f * scaleX, 1.0f, centerZ + 0.5f * scaleZ, 1.0f),
				          vec4(centerX + 0.5f * scaleX, 1.0f, centerZ + 0.5f * scaleZ, 1.0f),
				          vec4(centerX + 0.5f * scaleX, 1.0f, centerZ - 0.5f * scaleZ, 1.0f));

				if (checkCollision(pickingRay, planeAbs)) {
					selectedPosition = vec2(centerX, centerZ);
				}
			}
		}
	}

	void onboardingUpdate(float deltaTime, int &onboardingUpdateStage) {
		if (onboardingUpdateStage == 0) {
			camera.target = vec4(0.0f, 0.0f, 0.0f, 1.0f);
			camera.position = vec4(20.0f, 30.0f, 20.0f, 1.0f);
			camera.mode = CameraMode::LookAt;
			onboardingUpdateStage++;
		} else {
			camera.position.x += camera.position.z / 2 * deltaTime;
			camera.position.z += -camera.position.x / 2 * deltaTime;
		}
	}

	void handleMovement(float deltaTime) {
		float velocity = movementSpeed * deltaTime;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.move(CameraMovement::Forward, velocity);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.move(CameraMovement::Backward, velocity);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.move(CameraMovement::Left, velocity);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.move(CameraMovement::Right, velocity);
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
			camera.move(CameraMovement::Up, velocity);
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			camera.move(CameraMovement::Down, velocity);
	}

	void handleMouseMovement(GLFWwindow* window, double xpos, double ypos) {
		camera.handleMouseMovement(xpos, ypos, isLeftMouseButtonPressed);
	}

	void handleMouseClick(GLFWwindow *window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			isLeftMouseButtonPressed = true;
			
			if (currentStage == NetGuardStage::defenseDeployment) {
				AntiVirusUnit newUnit(vec4(selectedPosition.x, gridHeight + 0.5f, selectedPosition.y, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f));

				newUnit.sceneObject = antivirusSceneObject;
				defenseUnits.push_back(newUnit);
			}
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			isLeftMouseButtonPressed = false;
		}
	}

	void drawDefenseDeploymentScreen() {
		plane->scale = vec4(.9f, .9f, .9f, .9f);
		for (int x = -6; x <= 5; x++) {
			for (int z = -6; z <= 5; z++) {
				float centerX = x + 0.5f;
				float centerZ = z + 0.5f;

				plane->position = vec4(centerX, gridHeight + 0.01f, centerZ, 1.0f);

				if (centerX == selectedPosition.x && centerZ == selectedPosition.y) {
					plane->color = vec4(0.3f, 0.5f, 0.3f, 1.0f); // Highlighted color
				} else {
					plane->color = vec4(0.05f, 0.05f, 0.05f, 1.0f); // Default color
				}

				plane->drawObject(model_uniform, object_style_uniform, object_color_uniform);
			}
		}
	}
};

#endif // GAME_NET_GUARD_HPP