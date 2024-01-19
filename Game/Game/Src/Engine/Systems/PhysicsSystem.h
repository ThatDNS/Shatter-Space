// @file: PhysicsSystem.h
//
// @brief: Header file for PhysicsSystem class, responsible for all physics simulation.

#pragma once
#ifndef _PHYSICS_SYSTEM_H_
#define _PHYSICS_SYSTEM_H_

class RigidBody;

class PhysicsSystem
{
	DECLARE_SINGLETON(PhysicsSystem)
	
	float gravity = 0;
	std::list<RigidBody*> rigidBodies;

public:
	void SetGravity(float g) { gravity = g; }

	void AddRigidBody(RigidBody*);
	void RemoveRigidBody(RigidBody*);

protected:
	void Update(float);
	
	friend class Engine;
};

#endif // !_PHYSICS_SYSTEM_H_
