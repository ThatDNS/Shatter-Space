// @file: PhysicsSystem.h
//
// @brief: Header file for PhysicsSystem class, responsible for all physics simulation.

#include "stdafx.h"
#include "PhysicsSystem.h"
#include "Engine/Components/RigidBody.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Transform.h"

void PhysicsSystem::AddRigidBody(RigidBody* rb)
{
	rigidBodies.push_back(rb);
}

void PhysicsSystem::RemoveRigidBody(RigidBody* rb)
{
	rigidBodies.remove(rb);
}

void PhysicsSystem::Update(float deltaTime)
{
	for (RigidBody* rb : rigidBodies)
	{
		// Update velocity as per acceleration (v = u + at)
		rb->velocity += rb->instAcceleration * (deltaTime / 100.0f);
		// Instantaneous acceleration must be set to zero after it has been applied to the velocity
		rb->instAcceleration.Reset();
		
		// Apply gravity
		rb->velocity.y += gravity * (deltaTime / 100.0f);

		// Update position as per velocity
		bool didUpdate = rb->GetEntity()->Move(rb->velocity * (deltaTime / 100.0f), rb->collider);
		if (!didUpdate)
		{
			// There was collision. So velocity can be set to zero
			rb->velocity.Reset();
		}

		// Apply drag / friction if there's velocity
		if (rb->velocity.Magnitude() > 0.0f && rb->drag != 0)
		{
			float normalRxn = std::abs(rb->mass * gravity);
			float frictionForce = normalRxn * rb->drag;

			// Friction gets applied as acceleration, opposite to velocity
			Vector3 direction = -rb->velocity;
			direction.Normalize();
			rb->instAcceleration = direction * (frictionForce / rb->mass);

			// Upper bound acceleration such that the best it can do is stop object in single frame
			float actualAccMag = std::min(rb->instAcceleration.Magnitude(), rb->velocity.Magnitude() / (deltaTime / 100.0f));
			rb->instAcceleration.Normalize();
			rb->instAcceleration *= actualAccMag;
		}

	}
}
