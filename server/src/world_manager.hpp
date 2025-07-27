#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include "client_server_shared/network_messages.hpp"

class WorldManager {
  rp3d::PhysicsCommon _physicsCommon;
  rp3d::PhysicsWorld* _world = _physicsCommon.createPhysicsWorld();
  rp3d::BoxShape* _boxShape =
      _physicsCommon.createBoxShape(rp3d::Vector3(0.5, 0.5, 0.5));

 public:
  WorldManager();
  void add_cube(float x = 0,
                float y = 0,
                float z = 0);
  void simulate(float time);
  std::vector<Msg::CubeData> get_cube_data();
};