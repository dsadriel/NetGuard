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
		Sphere boundingSphere;

	GameUnit() : position(vec4(0.0f)), rotation(vec4(0.0f, 0.0f, 0.0f, 0.0f)), sceneObject(nullptr) {}
	GameUnit(vec4 pos, SceneObject* obj) : position(pos), rotation(vec4(0.0f, 0.0f, 0.0f, 0.0f)), sceneObject(obj) {}
	GameUnit(vec4 pos, vec4 rot) : position(pos), rotation(rot), sceneObject(nullptr) {}
	GameUnit(SceneObject* obj) : position(vec4(INFINITY)), rotation(vec4(0.0f, 0.0f, 0.0f, 0.0f)), sceneObject(obj) {}

	virtual void defenseAction() {}
	virtual void attackAction() {}

	virtual void draw(GLint model_uniform, GLint object_style_uniform, GLint object_color_uniform, GLint shading_mode_uniform ) {
		if (sceneObject) {
			sceneObject->position = position;
			sceneObject->rotation = rotation;
			sceneObject->drawObject(model_uniform, object_style_uniform, object_color_uniform, shading_mode_uniform);
		}
	}
};

class AntiVirusUnit : public GameUnit {
	public:
	
	AntiVirusUnit() : GameUnit() {}
	AntiVirusUnit(SceneObject* obj) : GameUnit(obj) {}

	void defenseAction() override {
		// Implement specific action for AntiVirusUnit
	}
};

#endif