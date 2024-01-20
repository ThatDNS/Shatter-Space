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
#include "Engine/Core/Logger.h"

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
		rb->velocity += rb->instAcceleration * (deltaTime / 1000.0f);
		// Instantaneous acceleration must be set to zero after it has been applied to the velocity
		rb->instAcceleration.Reset();

		// Apply gravity
		if (rb->applyGravity)
			rb->velocity.y += gravity * (deltaTime / 1000.0f);

		// If the object is not moving, there's nothing else to be done
		if (rb->velocity.Magnitude() == 0)
			continue;
		
		// Update position as per velocity
		bool didMove = rb->GetEntity()->Move(rb->velocity * (deltaTime / 1000.0f), rb->collider);
		if (didMove)
		{
			// Apply drag / friction
			// (This formula decreases velocity by drag percentage every second)
			rb->velocity -= (rb->velocity * rb->drag) / (1000.0f / deltaTime);
		}
		// Object didn't move, so there was a collision
		else
		{
			// Collision happens on object movement, so we have to move it first
			Vector3 moveDelta = rb->velocity * (deltaTime / 1000.0f);
			rb->GetEntity()->GetTransform().Translate(moveDelta);
			rb->collider->Callibrate();  // Adjust collider after transform change

			// Get collision normal
			Vector3 normal = CollisionSystem::Get().GetCollisionNormal(rb->collider);

			Logger::Get().Log("Not moving, collision normal: " + normal.ToString());

			if (normal.Magnitude() != 0)
			{
				// Change velocity in the direction of collision's normal vector
				if (normal.x != 0.0f)
					rb->velocity.x = -rb->velocity.x;
				if (normal.y != 0.0f)
					rb->velocity.y = -rb->velocity.y;
				if (normal.z != 0.0f)
					rb->velocity.z = -rb->velocity.z;

				// Some loss of energy on collision
				//rb->velocity = rb->velocity * 0.8f;
			}

			// Move the entity back & re-adjust the collider
			rb->GetEntity()->GetTransform().position -= moveDelta;
			rb->collider->Callibrate();
		}
	}
}
