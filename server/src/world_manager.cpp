#include "world_manager.hpp"
#include "reactphysics3d/mathematics/Transform.h"
#include "reactphysics3d/mathematics/Vector3.h"
#include <cstdlib>

WorldManager::WorldManager() {
  rp3d::Vector3 position(0, 0, 0);
  rp3d::Quaternion orientation = rp3d::Quaternion::identity();
  rp3d::Transform transform(position, orientation);
  rp3d::RigidBody* plane = _world->createRigidBody(transform);
  plane->setType(rp3d::BodyType::STATIC);
  rp3d::BoxShape* plane_shape =
      _physics_common.createBoxShape(rp3d::Vector3(100, 1, 100));
  plane->addCollider(plane_shape, rp3d::Transform::identity());
}

void WorldManager::add_cube(float x, float y, float z) {
  rp3d::Vector3 position(x, y, z);
  rp3d::Quaternion orientation = rp3d::Quaternion::identity();
  rp3d::Transform transform(position, orientation);
  rp3d::RigidBody* body = _world->createRigidBody(transform);

  body->addCollider(_cube_shape, rp3d::Transform::identity());
  _cubes.push_back(body);
}

void WorldManager::simulate(float time) {
  _world->update(time);
  if (rand()%5 == 0) {
    int r = rand()%(_cubes.size());
    auto c = _cubes[r];
    auto p = c->getTransform().getPosition();
    c->applyWorldForceAtCenterOfMass(rp3d::Vector3(-p.x*10, 200, -p.z*10));
    c->applyWorldTorque(rp3d::Vector3(10, 10, 10));
  }
}

std::vector<Msg::CubeData> WorldManager::get_cube_data() {
  std::vector<Msg::CubeData> data;

  for (int i = 0; i < _cubes.size(); i++) {
    rp3d::RigidBody* body = _cubes[i];
    rp3d::Transform t = body->getTransform();
    rp3d::Vector3 pos = t.getPosition();
    rp3d::Quaternion rot = t.getOrientation();

    data.push_back({float(pos.x), float(pos.y), float(pos.z), float(rot.x), float(rot.y), float(rot.z), float(rot.w)});
  }

  return data;
}