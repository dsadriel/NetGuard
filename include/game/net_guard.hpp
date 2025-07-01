#ifndef GAME_NET_GUARD_HPP
#define GAME_NET_GUARD_HPP

#include "game/camera.hpp"
#include "game/defense_unit.hpp"
#include "game/invasion_unit.hpp"
#include <external/glm/vec4.hpp>
#include <string>
#include <vector>

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
	std::vector<DefenseUnit> defenseUnits;
	// List of invasion units
	std::vector<InvasionUnit> invasionUnits;

	// Game state variables
	int currentInvasionWave = 0;
	int playerScore = 0;
	int playerLives = 3;

	float movementSpeed = 5.0f;

	int selectedUnitType = -1;

	GLFWwindow* window;

  public:
	Camera camera = Camera(glm::vec4(2.0f, 2.0f, 2.0f, 1.0f), -2.4f, -0.5f);

	// MARK: Initialization
	NetGuard() {
		currentStage = NetGuardStage::onboarding;
		defenseUnits.clear();
		invasionUnits.clear();
		currentInvasionWave = 0;
		playerScore = 0;
		playerLives = 3;
	}

	void configure(GLFWwindow* window){
		this->window = window;
	}

	NetGuardStage getCurrentStage() const { return currentStage; }

	std::string getCurrentStageString() const {
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
		// Update game logic based on the current stage
		switch (currentStage) {
		case NetGuardStage::onboarding:
			camera.target = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			camera.position = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);
			camera.mode = CameraMode::LookAt;
			break;
		case NetGuardStage::defenseDeployment:
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
		// Draw the current stage
		switch (currentStage) {
		case NetGuardStage::onboarding:
			// Draw onboarding screen
			break;
		case NetGuardStage::defenseDeployment:
			// Draw defense units screen
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
		camera.position = glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
		camera.mode = CameraMode::TopDown;
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
};

#endif // GAME_NET_GUARD_HPP