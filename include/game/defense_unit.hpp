#ifndef _DEFENSE_UNIT_HPP
#define _DEFENSE_UNIT_HPP

#include <external/glm/vec4.hpp>
#include "scene.hpp"

using namespace glm;

class GameUnit {
	private:
	public:
		SceneObject* sceneObject;
		vec4 position; // Position of the unit
		vec4 rotation; // Rotation of the unit
		int targetIndex;
		vec2 target;



	GameUnit(vec4 pos, vec4 dir) : position(pos), rotation(dir), sceneObject(nullptr) {};
	GameUnit() : position(vec4(0.0f)), rotation(vec4(0.0f, 0.0f, 1.0f, 0.0f)), sceneObject(nullptr) {}

	virtual void defenseAction() {}
	virtual void attackAction() {}
};

class AntiVirusUnit : public GameUnit {
	public:
	
	AntiVirusUnit(vec4 pos, vec4 dir) : GameUnit(pos, dir) {}
	AntiVirusUnit() : GameUnit() {}

	void defenseAction() override {
		// Implement specific action for AntiVirusUnit
	}
};

#endif