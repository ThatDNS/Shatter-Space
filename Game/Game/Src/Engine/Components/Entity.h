// @file: Entity.h
//
// @brief: Header file for the Entity class. Entities hold Components.

#pragma once

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Engine/Core/Object.h"

class Transform;
class Component;

class Entity final : public Object
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Entity, Object)

private:
	// Variables used for object pooling
	bool isPartOfObjectPool = false;
	bool isIdleInObjectPool = false;

	Transform* transform = nullptr;

	// Need to store the components as Object*, else TypeClass fails
	// Reason: When we create a component using TypeClass, we get Object*. 
	//         Force casting it to Component* causes GetDerivedClassName() to return "Component".
	std::list<Object*> components;
	std::list<Object*> componentsToAdd;
	std::list<Object*> componentsToRemove;

protected:
	Entity();
	Entity(std::string _guid);

	void Initialize() override;
	void Load(json::JSON& _entityJSON) override;
	void Update(float);
	void PreUpdate();
	void PostUpdate();

	void Destroy() override;

public:
	bool HasComponent(const std::string& componentClassName);
	Component* const GetComponent(STRCODE componentUId);
	Component* const GetComponent(const std::string& componentClassName);
	std::list<Component*> GetComponents(const std::string& componentClassName);

	Component* CreateComponent(const std::string& componentClassName);
	Component* LoadComponent(const std::string& componentClassName, json::JSON& componentJSON);

	bool RemoveComponent(const std::string& componentClassName);
	bool RemoveComponent(Component* _component);

	// ----------------------- Getters -----------------------------------

	Transform* GetTransform() const { return transform; }

	bool IsPartOfObjectPool() const { return isPartOfObjectPool; }
	bool IsIdleInObjectPool() const { return isIdleInObjectPool; }
	void MarkOccupiedInObjectPool() { isIdleInObjectPool = false; }
	void MarkIdleInObjectPool() { isIdleInObjectPool = true; }

	friend class Scene;
};

#endif // !_ENTITY_H_