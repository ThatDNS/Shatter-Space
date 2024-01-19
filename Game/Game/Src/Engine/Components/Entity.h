// @file: Entity.h
//
// @brief: Header file for the Entity class. Entities hold Components.

#pragma once

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Engine/Core/Object.h"
#include "Engine/Components/Transform.h"

class Component;
class Collider;
class EntityPool;

class Entity final : public Object
{
private:
	// An entity always has transform
	Transform transform;
	// Each entity must know about its source pool to return back to it
	EntityPool* sourcePool = nullptr;

	std::string name = "";

	std::list<Component*> components;
	std::list<Component*> componentsToAdd;
	std::list<Component*> componentsToRemove;

protected:
	Entity();
	Entity(std::string _guid);

	void Update(float);
	void PreUpdate();
	void PostUpdate();

	void Destroy() override;

public:
	void Initialize() override;

	bool HasComponent(ComponentType);
	bool HasRenderable();
	Component* const GetComponent(STRCODE componentUId);
	Component* const GetComponent(ComponentType);

	bool RemoveComponent(ComponentType);
	bool RemoveComponent(Component* _component);

	// Move an entity after checking collision. Returns if entity actually moved
	bool Move(Vector3& moveDelta, Collider* collider);
	// Rotate an entity in cartesian system along Z, after checking collision
	void CartesianRotationZ(Vector3& rotateDir, Collider* collider, float rotationSpeed);

	Transform& GetTransform() { return transform; }
	const std::string& GetName() const { return name; }
	void SetName(const std::string& n) { name = n; }

	friend class Scene;
	friend class EntityPool;
};

#endif // !_ENTITY_H_
