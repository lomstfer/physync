#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Cube {
  Cube(float x, float y, float z) {
    pos = glm::vec3(x, y, z);
    rot = glm::quat(1, 0, 0, 0);
  }

  Cube(float pos_x,
       float pos_y,
       float pos_z,
       float rot_x,
       float rot_y,
       float rot_z,
       float rot_w) {
    pos = glm::vec3(pos_x, pos_y, pos_z);
    rot = glm::quat(rot_w, rot_x, rot_y, rot_z);
  }

  glm::vec3 pos;
  glm::quat rot;
};