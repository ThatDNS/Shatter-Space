// @file: PhysicsSystem.h
//
// @brief: Header file for PhysicsSystem class, responsible for all physics simulation.

#include "stdafx.h"
#include "PhysicsSystem.h"
#include "Engine/Components/RigidBody.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Systems/CollisionSystem.h"

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

		// If the object is not moving, there's nothing else to be done
		if (rb->velocity.Magnitude() == 0)
			continue;

		// Update position as per velocity
		bool didMove = rb->GetEntity()->Move(rb->velocity * (deltaTime / 100.0f), rb->collider);
		if (didMove)
		{
			// Apply drag / friction
			if (rb->drag != 0)
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
		// Object didn't move, so there was a collision
		else
		{
			// Collision happens on object movement, so we have to move it first
			Vector3 moveDelta = rb->velocity * (deltaTime / 100.0f);
			rb->GetEntity()->GetTransform().Translate(moveDelta);
			rb->collider->Callibrate();  // Adjust collider after transform change

			// Get collision normal
			Vector3 normal = CollisionSystem::Get().GetCollisionNormal(rb->collider);

			// Change velocity in the direction of collision's normal vector
			if (normal.x != 0.0f)
				rb->velocity.x = -rb->velocity.x;
			if (normal.y != 0.0f)
				rb->velocity.y = -rb->velocity.y;
			if (normal.z != 0.0f)
				rb->velocity.z = -rb->velocity.z;

			// Move the entity back & re-adjust the collider
			rb->GetEntity()->GetTransform().position -= moveDelta;
			rb->collider->Callibrate();
		}
	}
}
