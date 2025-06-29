#ifndef _INVASION_UNIT_HPP
#define _INVASION_UNIT_HPP

#include <external/glm/vec4.hpp>

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

  #endif