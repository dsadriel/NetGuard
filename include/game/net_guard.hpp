#ifndef GAME_NET_GUARD_HPP
#define GAME_NET_GUARD_HPP

#include "collisions.hpp"
#include "game/camera.hpp"
#include "game/defense_unit.hpp"
#include "game/invasion_unit.hpp"
#include "globals.hpp"
#include "scene.hpp"
#define GLFW_INCLUDE_NONE
#include "utils/text_rendering.hpp"
#include <external/GLFW/glfw3.h>
#include <external/glm/vec4.hpp>
#include <string>
#include <vector>
// #define M_PI 3.14159265358979323846

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
	vector<GameUnit> availableDefenseUnits;
	vector<GameUnit> defenseUnits;
	// List of invasion units
	vector<GameUnit> invasionUnits;

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
	GLint shading_mode_uniform;

	bool isLeftMouseButtonPressed = false;
	bool isPKeyPressed = false;
	bool hasInvasionStarted = false;

	vec2 selectedPosition = vec2(-INFINITY, -INFINITY);

	const int gridHeight = 1.0f;

	// Game SceneObjects
	SceneObject *map = nullptr;
	SceneObject *board = nullptr;
	SceneObject *cat = nullptr;
	SceneObject *plane = nullptr;
	SceneObject *antivirusSceneObject = nullptr;

	// Transition variables (Curva de bezier)
	vec4 bezier_p0_pos = vec4(0.0f);
    vec4 bezier_p1_pos = vec4(0.0f);
    vec4 bezier_p2_pos = vec4(0.0f);
    vec4 bezier_p3_pos = vec4(0.0f);

    vec4 bezier_p0_target = vec4(0.0f); 
    vec4 bezier_p1_target = vec4(0.0f);
    vec4 bezier_p2_target = vec4(0.0f);
    vec4 bezier_p3_target = vec4(0.0f);

    float transition_timer = 0.0f;
    float transition_duration = 3.0f;
    bool in_transition = false;
	BoundingBox mapPlaneBB = BoundingBox(vec4(0.0f, 12.3f, 0.0f, 1.0f), vec4(60.0f, 21.0f, 60.0f, 1.0f));
	vec4 lastCameraPos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 lastUnitPos = vec4(0.0f, 0.0f, 0.0f, 1.0f);


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

	void link(GLFWwindow *window, GLint model_uniform, GLint object_style_uniform, GLint object_color_uniform,
	          GLint shading_mode_uniform) {
		this->window = window;
		this->model_uniform = model_uniform;
		this->object_style_uniform = object_style_uniform;
		this->object_color_uniform = object_color_uniform;
		this->shading_mode_uniform = shading_mode_uniform;
	}

	void linkSceneObjects(SceneObject *map, SceneObject *board, SceneObject *cat, SceneObject *plane,
	                      SceneObject *antivirusSceneObject) {
		this->map = map;
		this->board = board;
		this->cat = cat;
		this->plane = plane;
		this->antivirusSceneObject = antivirusSceneObject;

		availableDefenseUnits.clear();
		availableDefenseUnits.push_back(AntiVirusUnit(antivirusSceneObject));
		availableDefenseUnits.push_back(AntiVirusUnit(antivirusSceneObject));
		availableDefenseUnits.push_back(AntiVirusUnit(antivirusSceneObject));
		availableDefenseUnits.push_back(AntiVirusUnit(antivirusSceneObject));
		availableDefenseUnits.push_back(AntiVirusUnit(antivirusSceneObject));
		availableDefenseUnits.push_back(AntiVirusUnit(antivirusSceneObject));
		availableDefenseUnits.push_back(AntiVirusUnit(antivirusSceneObject));
		availableDefenseUnits.push_back(AntiVirusUnit(antivirusSceneObject));
		availableDefenseUnits.push_back(AntiVirusUnit(antivirusSceneObject));
		availableDefenseUnits.push_back(AntiVirusUnit(antivirusSceneObject));
		availableDefenseUnits.push_back(AntiVirusUnit(antivirusSceneObject));
		availableDefenseUnits.push_back(AntiVirusUnit(antivirusSceneObject));
	}

	// MARK: Stage Management
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

	void nextStage() {
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

	// MARK: Update

	void update(float deltaTime) {
		static int onboardingUpdateStage = 0;

		if (in_transition) 
		{
			transition_timer += deltaTime;
			float t = transition_timer / transition_duration;

			if (t >= 1.0f) 
			{
				t = 1.0f;
				in_transition = false; 

				camera.position = bezier_p3_pos;
				camera.target = bezier_p3_target;
			} 

			camera.position = calculateBezierPoint(t, bezier_p0_pos, bezier_p1_pos, bezier_p2_pos, bezier_p3_pos);
			camera.target = calculateBezierPoint(t, bezier_p0_target, bezier_p1_target, bezier_p2_target, bezier_p3_target);
			camera.mode = CameraMode::LookAt;
		}

    else {
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
				updateInvasionPhase(deltaTime);
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

		// If in debug mode, allow stage change with 'P' key (only trigger once per press)
		bool currentPKeyState = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
		if (g_DebugMode && currentPKeyState && !isPKeyPressed) {
			nextStage();
		}
		isPKeyPressed = currentPKeyState;
	}

	// MARK: Draw
	void draw() {
		map->drawObject(model_uniform, object_style_uniform, object_color_uniform, shading_mode_uniform);
		board->drawObject(model_uniform, object_style_uniform, object_color_uniform, shading_mode_uniform);

		for (auto &defenseUnit : defenseUnits) {
			if (defenseUnit.sceneObject != nullptr) {
				defenseUnit.draw(model_uniform, object_style_uniform, object_color_uniform, shading_mode_uniform);
			}
		}

		switch (currentStage) {
		case NetGuardStage::onboarding:
			drawOnboardingScreen();
			break;
		case NetGuardStage::defenseDeployment:
			drawDefenseDeploymentScreen();
			break;
		case NetGuardStage::invasionPhase:
			drawInvasionPhase();
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

	// MARK: Input Handling
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
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			camera.move(CameraMovement::Up, velocity);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			camera.move(CameraMovement::Down, velocity);
	}

	void handleMouseMovement(GLFWwindow *window, double xpos, double ypos) {
		camera.handleMouseMovement(xpos, ypos, isLeftMouseButtonPressed);
	}

	void handleMouseClick(GLFWwindow *window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			isLeftMouseButtonPressed = true;

			if (currentStage == NetGuardStage::defenseDeployment) {
				defenseDeploymentAddUnit();
			}
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			isLeftMouseButtonPressed = false;
		}
	}

	// MARK: Path definition
	bool isPositionInPath(int x, int z) {
		return (z % 2 != 0 && abs(x) != 6 && !(x > 0 && z == 5)) || (x == 5 && (z == 2 || z == -2)) ||
		       (x == -5 && (abs(z) == 4 || z == 0)) || (x == 0 && z == 6) || (x == 5 && z == -6);
	}

	vector<vec2> getPathPoints() {
		vector<vec2> points = {{5.5f, -5.5f}, {5.5f, -4.5f},  {-4.5f, -4.5f}, {-4.5f, -2.5f}, {5.5f, -2.5f},
		                       {5.5f, -0.5f}, {-4.5f, -0.5f}, {-4.5f, 1.5f},  {5.5f, 1.5f},   {5.5f, 3.5f},
		                       {-4.5f, 3.5f}, {-4.5f, 5.5f},  {0.5f, 5.5f},   {0.5f, 6.5f}};
		return points;
	}

	// MARK: Onboarding
	void onboardingUpdate(float deltaTime, int &onboardingUpdateStage) {
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			currentStage = NetGuardStage::defenseDeployment;
			vec4 onboardingCurrentPos = camera.position;
            vec4 onboardingCurrentTarget = camera.target;

            // Posição final
            vec4 defenseStartPos = vec4(-12.0f, 15.0f, 0.0f, 1.0f);
            vec4 defenseStartTarget = vec4(0.0f, 0.0f, 0.0f, 1.0f);

            // Realizar ajustes 
            vec4 p1_pos = onboardingCurrentPos + vec4(-5.0f, 10.0f, -5.0f, 0.0f); 
            vec4 p2_pos = defenseStartPos + vec4(0.0f, 5.0f, 0.0f, 0.0f);

            // Pontos de controle para o alvo (para onde a câmera está olhando)
            vec4 p1_target = onboardingCurrentTarget; 
            vec4 p2_target = defenseStartTarget; 

			// Duração da transição em SEGUNDOS
            float transition_time = 3.0f; 

            startCameraTransition(
                onboardingCurrentPos, p1_pos, p2_pos, defenseStartPos,
                onboardingCurrentTarget, p1_target, p2_target, defenseStartTarget,
                transition_time
            );
			
		}

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

	void drawOnboardingScreen() {
		// Show the instructions for the onboarding stage
		TextRendering_PrintStringC(window, "Press ENTER to start the defense deployment phase", -.5f, -.75f,
		                           glm::vec3(1.0f, 1.0f, 1.0f), 1.2f);
	}

	// MARK: Defense Deployment
	void defenseDeploymentUpdate() {
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && defenseUnits.size() >= 1) {
			hasInvasionStarted = false;
			currentStage = NetGuardStage::invasionPhase;
		}
			
		selectedPosition = vec2(INFINITY, INFINITY);

		// Handle grid selection logic
		double cursorX, cursorY;
		glfwGetCursorPos(window, &cursorX, &cursorY);
		Ray pickingRay = camera.getPickingRay(g_WindowWidth, g_WindowHeight, cursorX, cursorY);

		// Check collision for each grid cell
		for (int x = -6; x <= 6; x++) {
			for (int z = -6; z <= 6; z++) {
				float centerX = x + 0.5f;
				float centerZ = z + 0.5f;

				Plane planeAbs = Plane(
				    vec4(centerX - 0.5f, 1.0f, centerZ - 0.5f, 1.0f), vec4(centerX - 0.5f, 1.0f, centerZ + 0.5f, 1.0f),
				    vec4(centerX + 0.5f, 1.0f, centerZ + 0.5f, 1.0f), vec4(centerX + 0.5f, 1.0f, centerZ - 0.5f, 1.0f));

				if (checkCollision(pickingRay, planeAbs) && !isPositionInPath(x, z)) {
					selectedPosition = vec2(centerX, centerZ);
				}
			}
		}
	}

	void defenseDeploymentAddUnit() {
		if (selectedPosition.x != INFINITY && selectedPosition.y != INFINITY) {
			if (isPositionInPath(selectedPosition.x - 0.5f, selectedPosition.y - 0.5f)) {
				printf("X: %f, Y: %f\n", selectedPosition.x, selectedPosition.y);
				return; // Cannot place defense unit in path
			}

			for (const auto &defenseUnit : defenseUnits) {
				if (defenseUnit.position.x == selectedPosition.x && defenseUnit.position.z == selectedPosition.y) {
					printf("Position already occupied by another defense unit.\n");
					return; // Position already occupied
				}
			}

			if (!availableDefenseUnits.empty()) {
				GameUnit newUnit = availableDefenseUnits.back();
				availableDefenseUnits.pop_back();
				newUnit.position = vec4(selectedPosition.x, gridHeight + .6f, selectedPosition.y, 1.0f);
				newUnit.boundingBox.origin = vec4(selectedPosition.x, gridHeight + 0.6f, selectedPosition.y, 1.0f);
				defenseUnits.push_back(newUnit);
			} else {
				printf("No available defense units to place.\n");
			}
		}
	}

	void drawDefenseDeploymentScreen() {
		plane->scale = vec4(.9f, .9f, .9f, .9f);
		for (int x = -6; x <= 6; x++) {
			for (int z = -6; z <= 6; z++) {
				float centerX = x + 0.5f;
				float centerZ = z + 0.5f;

				plane->position = vec4(centerX, gridHeight + 0.02f, centerZ, 1.0f);

				vec4 pathColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
				vec4 availablePositionColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
				vec4 selectedPositionColor = vec4(0.3f, 0.5f, 0.3f, 1.0f);
				vec4 notPermitedColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

				if (isPositionInPath(x, z)) {
					plane->color = pathColor;
				} else {
					if (selectedPosition.x == centerX && selectedPosition.y == centerZ) {
						plane->color = availableDefenseUnits.empty() ? notPermitedColor : selectedPositionColor;
					} else {
						plane->color = availablePositionColor;
					}
				}

				plane->drawObject(model_uniform, object_style_uniform, object_color_uniform, shading_mode_uniform);
			}
		}

		// Draw available defense units
		int gridSize = 4, startX = 9, startZ = -6, idx = 0;

		int availableCount = availableDefenseUnits.size();

		// If there are available defense units, draw them in the grid
		if (availableCount > 0) {
			// If selected position is valid, draw the last available defense unit there
			if (selectedPosition.x != INFINITY && selectedPosition.y != INFINITY) {
				if (availableDefenseUnits.back().sceneObject != nullptr) {
					availableDefenseUnits.back().position =
					    vec4(selectedPosition.x, gridHeight + 0.6f, selectedPosition.y, 1.0f);
					availableDefenseUnits.back().draw(model_uniform, object_style_uniform, object_color_uniform,
					                                  shading_mode_uniform);
				}

				// Decrease the count of available units, so it won't be drawn again
				availableCount--;
			}

			// Draw the available defense units in a grid
			for (int j = 0; j < gridSize && idx < availableCount; ++j) {
				for (int i = 0; i < gridSize && idx < availableCount; ++i, ++idx) {
					auto &availableDU = availableDefenseUnits[idx];
					if (availableDU.sceneObject != nullptr) {
						float posX = startX + i + 0.5f;
						float posZ = startZ + j + 0.5f;
						availableDU.position = vec4(posX, 0.6f, posZ, 1.0f);
						availableDU.draw(model_uniform, object_style_uniform, object_color_uniform,
						                 shading_mode_uniform);
					}
				}
			}
		}

		// Unit placement message
		if (availableDefenseUnits.size() == 0) {
			TextRendering_PrintStringC(window, "No available defense units to place", -0.95f, -0.90f,
			                           glm::vec3(1.0f, 0.0f, 0.0f), 1.2f);
		} else {
			TextRendering_PrintStringC(window, "Available defense units: " + to_string(availableDefenseUnits.size()),
			                           -0.95f, -0.85f, glm::vec3(1.0f, 1.0f, 1.0f), 1.2f);
			TextRendering_PrintStringC(window, "Click to place a defense unit", -0.95f, -0.90f,
			                           glm::vec3(1.0f, 1.0f, 1.0f), 1.2f);
		}

		// Start invasion phase message, if there are defense units placed
		if (defenseUnits.size() > 0)
			TextRendering_PrintStringC(window, "Press ENTER to start the invasion phase", -0.95f, -0.95f,
			                           glm::vec3(1.0f, 1.0f, 1.0f), 1.2f);
	}

	void startCameraTransition(vec4 p0_pos, vec4 p1_pos, vec4 p2_pos, vec4 p3_pos, vec4 p0_target, 
	vec4 p1_target, vec4 p2_target, vec4 p3_target, float duration)
	{
		this->bezier_p0_pos = p0_pos;
		this->bezier_p1_pos = p1_pos;
		this->bezier_p2_pos = p2_pos;
		this->bezier_p3_pos = p3_pos;

		this->bezier_p0_target = p0_target;
		this->bezier_p1_target = p1_target;
		this->bezier_p2_target = p2_target;
		this->bezier_p3_target = p3_target;

		this->transition_duration = duration;
		this->transition_timer = 0.0f;
		this->in_transition = true;
		this->camera.mode = CameraMode::LookAt;
	}

	vec4 calculateBezierPoint(float t, vec4 p0, vec4 p1, vec4 p2, vec4 p3) {
    float oneMinusT = 1.0f - t;
	return 	pow(oneMinusT, 3.0f) * p0 + 
			(3.0f * t * pow(oneMinusT, 2.0f)) * p1 + 
			(3.0f * pow(t, 2.0f) * oneMinusT) * p2 + 
			pow(t, 3.0f) * p3;
	}

	// MARK: Invasion Phase
	void updateInvasionPhase(float deltaTime) {
		static int currentTargetIndex = 0;
		if (!hasInvasionStarted) {

			// Set the camera position and orientation for the invasion phase
			camera.position = vec4(6.0f, 6.0f, -10.0f, 1.0f);
			camera.changePitch(-0.1f);
			camera.changeYaw(3*M_PI/4);
			camera.mode = CameraMode::Free;
			hasInvasionStarted = true;

			// Clear the invasion units at the start of the invasion phase
			invasionUnits.clear();
			currentTargetIndex = 0;
		}

		vector<vec2> targets = getPathPoints();

		camera.boundingSphere.center = camera.position;
		camera.boundingSphere.radius = 0.5f;

		bool hadMapAndCameraCollision = checkCollision(camera.boundingSphere, mapPlaneBB);
		bool hasUnitCollision = false;

		for (const auto &defenseUnit : defenseUnits) {
			if(defenseUnit.sceneObject != nullptr && checkCollision(camera.boundingSphere, defenseUnit.boundingBox)){
				hasUnitCollision = true;
				break;
			}			
		}

		for (const auto &invasionUnit : invasionUnits) {
			if(invasionUnit.sceneObject != nullptr && checkCollision(camera.boundingSphere, invasionUnit.boundingBox)){
				hasUnitCollision = true;
				break;
			}			
		}

		if(!hadMapAndCameraCollision || hasUnitCollision){
			camera.position = lastCameraPos;
		}
		else{
			lastCameraPos = camera.position;
		}

		if (invasionUnits.empty()) {
			vec2 firstPosition = getPathPoints().front();
			GameUnit invasionUnit(vec4(firstPosition.x, gridHeight + 0.53f, firstPosition.y, 1.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f));
			invasionUnit.sceneObject = cat;
			invasionUnit.sceneObject->scale = vec4(0.7f, 0.7f, 0.7f, 1.0f);
			invasionUnits.push_back(invasionUnit);
		}

		for (auto &invasionUnit : invasionUnits) {
			if (invasionUnit.sceneObject != nullptr) {
				vec2 target = targets[currentTargetIndex];

				vec2 currentPosition = vec2(invasionUnit.position.x, invasionUnit.position.z);

				vec2 toTarget = target - currentPosition;
				vec2 direction = length(toTarget) > 0.0001f ? normalize(toTarget) : vec2(0.0f, 0.0f);

				vec2 newPos = currentPosition + direction * 0.5f * deltaTime;
				invasionUnit.position = vec4(newPos.x, invasionUnit.position.y, newPos.y, 1.0f);

				float angle = atan2(direction.x, direction.y);
				invasionUnit.rotation = vec4(0.0f, angle, 0.0f, 1.0f);

				if (length(toTarget) < 0.1f) {
					invasionUnit.position = vec4(target.x, invasionUnit.position.y, target.y, 1.0f);
					currentTargetIndex = (currentTargetIndex + 1) % targets.size();
				}
				
				invasionUnit.boundingBox.origin = invasionUnit.position;
			}
			bool hadUnitAndCameraCollison = checkCollision(camera.boundingSphere, invasionUnit.boundingBox);
				if(hadUnitAndCameraCollison)
				{
					invasionUnit.position = lastUnitPos;
				}
				else 
				{
					lastUnitPos = invasionUnit.position;
				}
		}
		
	}

	void drawInvasionPhase() {

		for (auto &invasionUnit : invasionUnits) {
			if (invasionUnit.sceneObject != nullptr) {
				invasionUnit.sceneObject->position = invasionUnit.position;
				invasionUnit.sceneObject->rotation = invasionUnit.rotation;
				invasionUnit.sceneObject->drawObject(model_uniform, object_style_uniform, object_color_uniform, shading_mode_uniform);
			}
		}

		// Draws a message that says that the invasion phase isn't yet implemented
		TextRendering_PrintStringC(window, "Invasion phase is not yet implemented", -0.95f, -0.90f,  glm::vec3(1.0f, 0.5f, 0.5f), 1.2f);
		TextRendering_PrintStringC(window, "Press ESC to close the game", -0.95f, -0.95f,  glm::vec3(1.0f, 1.0f, 1.0f), 1.2f);
							
	}
};

#endif // GAME_NET_GUARD_HPP