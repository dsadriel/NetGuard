#ifndef _DEFENSE_UNIT_HPP
#define _DEFENSE_UNIT_HPP

#include "game_unit.hpp"


class DefenseUnit : public GameUnit {
	public:
	
	DefenseUnit() : GameUnit() {}
	DefenseUnit(SceneObject* obj) : GameUnit(obj) {}

	virtual void defenseAction() {}
};

class AntiVirusUnit : public DefenseUnit {
	public:
	
	AntiVirusUnit() : DefenseUnit() {}
	AntiVirusUnit(SceneObject* obj) : DefenseUnit(obj) {}

	void defenseAction() override {
		// Implement specific action for AntiVirusUnit
	}
};

#endif