#pragma once

#include <vector>
#include <reactphysics3d/reactphysics3d.h>

class WorldManager
{
    rp3d::PhysicsCommon _physicsCommon;
    rp3d::PhysicsWorld *_world = _physicsCommon.createPhysicsWorld();
    rp3d::BoxShape *_boxShape = _physicsCommon.createBoxShape(rp3d::Vector3(1, 1, 1));

public:
    void add_cube(float x, float y, float z, float euler_x, float euler_y, float euler_z);
    void simulate(float time);
};