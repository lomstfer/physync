#pragma once
#include "glm/ext/quaternion_common.hpp"
#include "glm/ext/vector_float3.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Cube {
  Cube(float x, float y, float z) {
    pos = glm::vec3(x, y, z);
    rot = glm::quat(1, 0, 0, 0);
  }

  Cube(float pos_x, float pos_y, float pos_z, float rot_x, float rot_y,
       float rot_z, float rot_w) {
    pos = glm::vec3(pos_x, pos_y, pos_z);
    rot = glm::quat(rot_w, rot_x, rot_y, rot_z);
  }

  Cube(glm::vec3 pos, glm::quat rot) {
    this->pos = pos;
    this->rot = rot;
  }

  glm::vec3 pos;
  glm::quat rot;
};

struct CubeSnapshot {
  Cube data;
  double timestamp;
  static Cube lerp(CubeSnapshot c0, CubeSnapshot c1, float timestamp) {
    float t = (timestamp - c0.timestamp) / (c1.timestamp - c0.timestamp);
    glm::vec3 pos = glm::mix(c0.data.pos, c1.data.pos, t);
    glm::quat rot = glm::slerp(c0.data.rot, c1.data.rot, t);

    return Cube(pos, rot);
  }
};