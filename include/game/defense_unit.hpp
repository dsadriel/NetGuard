#ifndef _DEFENSE_UNIT_HPP
#define _DEFENSE_UNIT_HPP

#include <glm/vec4.hpp>

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

#endif