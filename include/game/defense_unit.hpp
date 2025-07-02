#ifndef _DEFENSE_UNIT_HPP
#define _DEFENSE_UNIT_HPP

#include <external/glm/vec4.hpp>
#include "scene.hpp"

using namespace glm;

class DefenseUnit {
  private:
	vec4 position;  // Position of the defense unit
	vec4 direction; // Direction the unit is facing
  public:
  	SceneObject* sceneObject;

	DefenseUnit(vec4 pos, vec4 dir) : position(pos), direction(dir), sceneObject(nullptr) {};
	DefenseUnit() : position(vec4(0.0f)), direction(vec4(0.0f, 0.0f, 1.0f, 0.0f)), sceneObject(nullptr) {}

	// Getters for position and direction
	vec4 getPosition() const { return position; }
	vec4 getDirection() const { return direction; }

	// Setters for position and direction
	void setPosition(const vec4 &pos) { position = pos; }
	void setDirection(const vec4 &dir) { direction = dir; }

	virtual void defenseAction() {}
};

class AntiVirusUnit : public DefenseUnit {
	public:
	
	AntiVirusUnit(vec4 pos, vec4 dir) : DefenseUnit(pos, dir) {}
	AntiVirusUnit() : DefenseUnit() {}

	void defenseAction() override {
		// Implement specific action for AntiVirusUnit
	}
};

#endif