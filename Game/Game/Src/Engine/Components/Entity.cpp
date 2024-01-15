// @file: Entity.cpp
//
// @brief: Cpp file for the Entity class. Entities hold Components.

#include "stdafx.h"
#include "App/app.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/CollisionSystem.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Component.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/Collider.h"

Entity::Entity()
{
	// Mark it as an entity
	isEntity = true;
}

Entity::Entity(std::string _guid) : Object(_guid)
{
}

void Entity::Initialize()
{
	// Initialize happens before first PreUpdate. So it must be called on componentsToAdd
	for (Component* component : componentsToAdd)
	{
		component->Initialize();
	}
}

void Entity::PreUpdate()
{
	for (Component* component : componentsToAdd)
	{
		components.push_back(component);
	}
	componentsToAdd.clear();
} 

void Entity::Update(float deltaTime)
{
	for (Component* component : components)
	{
		if (component->IsActive())
		{
			component->Update(deltaTime);
		}
	}
}  

void Entity::PostUpdate()
{
	for (Component* component : componentsToRemove)
	{
		components.remove(component);
		delete component;
	}
	componentsToRemove.clear();
}

void Entity::Destroy()
{
	for (Component* component : components)
	{
		component->Destroy();
		delete component;
	}
	components.clear();
}

bool Entity::HasComponent(ComponentType componentType)
{
	for (Component* component : components)
	{
		if (component->type == componentType)
		{
			return true;
		}
	}
	// In case this runs before PreUpdate, check in componentsToAdd
	for (Component* component : componentsToAdd)
	{
		if (component->type == componentType)
		{
			return true;
		}
	}
	return false;
}

bool Entity::HasRenderable()
{
	// TODO: Find a way to avoid this hard-coding
	return (HasComponent(SpriteC) || HasComponent(MeshRendererC));
}

Component* const Entity::GetComponent(STRCODE componentUId)
{
	for (Component* component : components)
	{
		if (component->GetUid() == componentUId)
		{
			return component;
		}
	}
	// If this ran before preUpdate, component would be in componentsToAdd
	for (Component* component : componentsToAdd)
	{
		if (component->GetUid() == componentUId)
		{
			return component;
		}
	}
	return nullptr;
}

Component* const Entity::GetComponent(ComponentType componentType)
{
	for (Component* component : components)
	{
		if (component->type == componentType)
		{
			return component;
		}
	}
	// If this ran before preUpdate, component would be in componentsToAdd
	for (Component* component : componentsToAdd)
	{
		if (component->type == componentType)
		{
			return component;
		}
	}
	return nullptr;
}

bool Entity::RemoveComponent(ComponentType componentType)
{
	for (Component* component : components)
	{
		if (component->type == componentType)
		{
			componentsToRemove.push_back(component);
			return true;
		}
	}
	return false;
}

bool Entity::RemoveComponent(Component* _component)
{
	for (Component* component : components)
	{
		if (component->GetUid() == _component->GetUid())
		{
			componentsToRemove.push_back(component);
			return true;
		}
	}
	return false;
}

void Entity::Move(Vector3& moveDelta, Collider* collider)
{
	// Move the entity
	transform.Translate(moveDelta);

	// Check if this caused collision
	collider->Callibrate();
	if (CollisionSystem::Get().CheckCollision(collider))
	{
		// Move the entity back
		transform.position -= moveDelta;

		// Recallibrate
		collider->Callibrate();
	}
}

void Entity::CartesianRotationZ(Vector3& rotateDir, Collider* collider, float rotationSpeed)
{
	// Convert to radians
	float rad = std::atan2f(rotateDir.x, rotateDir.y);
	if (rad < 0.0f)  // [-PI, PI] -> [0, 2*PI]
		rad += 2.0f * PI;

	// Cache it to rollback in case of collision
	Vector3 prevRotation = transform.rotation;

	// Rotate the entity
	Vector3 newRotation{ transform.rotation.x, transform.rotation.y, rad };
	if (std::abs(std::abs(transform.rotation.z) - std::abs(newRotation.z)) > 3.0f * PI / 2.0f)
		transform.rotation.z = newRotation.z;  // Don't lerp if change is too drastic
	else
		Vector3::Lerp(transform.rotation, newRotation, rotationSpeed);

	// Check if this caused collision
	collider->Callibrate();
	if (CollisionSystem::Get().CheckCollision(collider))
	{
		// Rotate the entity back
		transform.rotation = prevRotation;

		// Recallibrate
		collider->Callibrate();
	}
}
