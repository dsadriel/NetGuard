#ifndef GAME_NET_GUARD_HPP
#define GAME_NET_GUARD_HPP

#include "game/camera.hpp"
#include <glm/vec4.hpp>
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

class DefenseUnit {
  private:
	glm::vec4 position;  // Position of the defense unit
	glm::vec4 direction; // Direction the unit is facing
  public:
	DefenseUnit(glm::vec4 pos, glm::vec4 dir) : position(pos), direction(dir) {};

	// Getters for position and direction
	glm::vec4 getPosition() const { return position; }
	glm::vec4 getDirection() const { return direction; }

	// Setters for position and direction
	void setPosition(const glm::vec4 &pos) { position = pos; }
	void setDirection(const glm::vec4 &dir) { direction = dir; }

	virtual void defenseAction() {}
};

class InvasionUnit {
  private:
	glm::vec4 position;  // Position of the invasion unit
	glm::vec4 direction; // Direction the unit is facing
  public:
	InvasionUnit(glm::vec4 pos, glm::vec4 dir) : position(pos), direction(dir) {};

	// Getters for position and direction
	glm::vec4 getPosition() const { return position; }
	glm::vec4 getDirection() const { return direction; }

	// Setters for position and direction
	void setPosition(const glm::vec4 &pos) { position = pos; }
	void setDirection(const glm::vec4 &dir) { direction = dir; }

	virtual void invasionAction() {}
};

class NetGuard {
  private:
	NetGuardStage currentStage = NetGuardStage::onboarding;

	// List of defense units
	std::vector<DefenseUnit> defenseUnits;
	// List of invasion units
	std::vector<InvasionUnit> invasionUnits;

	// Game state variables
	int currentInvasionWave = 0;
	int playerScore = 0;
	int playerLives = 3;

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
			// Handle onboarding logic
			break;
		case NetGuardStage::defenseDeployment:
			// Handle adding defense units logic
			break;
		case NetGuardStage::invasionPhase:
			// Handle invasion phase logic
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
};

#endif // GAME_NET_GUARD_HPP