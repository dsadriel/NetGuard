#ifndef _INVASION_UNIT_HPP
#define _INVASION_UNIT_HPP

#include "game_unit.hpp"

class InvasionUnint : public GameUnit {
  public:
	InvasionUnint() : GameUnit() {}
	InvasionUnint(SceneObject *obj) : GameUnit(obj) {}

	virtual void attackAction() {}
};

#endif