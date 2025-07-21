#include "world_manager.hpp"

void WorldManager::add_cube(float x = 0, float y = 0, float z = 0, float euler_x = 0, float euler_y = 0, float euler_z = 0)
{
    rp3d::Vector3 position(x, y, z);
    rp3d::Quaternion orientation = rp3d::Quaternion::fromEulerAngles(euler_x, euler_y, euler_z);
    rp3d::Transform transform(position, orientation);
    rp3d::RigidBody *body = _world->createRigidBody(transform);

    rp3d::BoxShape *boxShape = _physicsCommon.createBoxShape(rp3d::Vector3(1, 1, 1));
    body->addCollider(boxShape, rp3d::Transform::identity());
}

void WorldManager::simulate(float time)
{
    _world->update(time);
}