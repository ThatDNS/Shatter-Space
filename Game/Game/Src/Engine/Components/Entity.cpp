// @file: Entity.cpp
//
// @brief: Cpp file for the Entity class. Entities hold Components.

#include "stdafx.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Component.h"
#include "Engine/Components/Transform.h"

IMPLEMENT_DYNAMIC_CLASS(Entity)

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
	for (Object* component : componentsToAdd)
	{
		((Component*) component)->Initialize();
	}
}

void Entity::Load(json::JSON& entityData)
{
	if (entityData.hasKey("Name"))
	{
		name = entityData["Name"].ToString();
	}

	if (entityData.hasKey("GUID"))
	{
		guid = entityData["GUID"].ToString();
		uid = GetHashCode(guid.c_str());
	}

	// Load the components
	if (entityData.hasKey("Components"))
	{
		for (json::JSON& componentJSON : entityData["Components"].ArrayRange())
		{
			std::string componentClassName = componentJSON["ClassName"].ToString();
			if (componentClassName == "Transform")
			{
				transform.Load(componentJSON["ClassData"]);
			}
			else
			{
				Component* component = GetComponent(componentClassName);
				component->Load(componentJSON["ClassData"]);
			}
		}
	}
}

void Entity::PreUpdate()
{
	for (Object* component : componentsToAdd)
	{
		components.push_back(component);
	}
	componentsToAdd.clear();
} 

void Entity::Update(float deltaTime)
{
	for (Object* component : components)
	{
		if (component->IsActive())
		{
			static_cast<Component*>(component)->Update(deltaTime);
		}
	}
}  

void Entity::PostUpdate()
{
	for (Object* component : componentsToRemove)
	{
		components.remove(component);
		delete component;
	}
	componentsToRemove.clear();
}

void Entity::Destroy()
{
	for (Object* component : components)
	{
		component->Destroy();
		delete component;
	}
	components.clear();
}

bool Entity::HasComponent(const std::string& componentClassName)
{
	for (Object* component : components)
	{
		if (component->GetDerivedClassName() == componentClassName)
		{
			return true;
		}
	}
	// In case this runs before PreUpdate, check in componentsToAdd
	for (Object* component : componentsToAdd)
	{
		if (component->GetDerivedClassName() == componentClassName)
		{
			return true;
		}
	}
	return false;
}

bool Entity::HasRenderable()
{
	// TODO: Find a way to avoid this hard-coding
	return (HasComponent("Sprite") || HasComponent("MeshRenderer"));
}

Component* const Entity::GetComponent(STRCODE componentUId)
{
	for (Object* component : components)
	{
		if (component->GetUid() == componentUId)
		{
			return static_cast<Component*>(component);
		}
	}
	// If this ran before preUpdate, component would be in componentsToAdd
	for (Object* component : componentsToAdd)
	{
		if (component->GetUid() == componentUId)
		{
			return static_cast<Component*>(component);
		}
	}
	return nullptr;
}

Component* const Entity::GetComponent(const std::string& componentClassName)
{
	for (Object* component : components)
	{
		if (component->GetDerivedClassName() == componentClassName)
		{
			return static_cast<Component*>(component);
		}
	}
	// If this ran before preUpdate, component would be in componentsToAdd
	for (Object* component : componentsToAdd)
	{
		if (component->GetDerivedClassName() == componentClassName)
		{
			return static_cast<Component*>(component);
		}
	}
	return nullptr;
}

std::list<Component*> Entity::GetComponents(const std::string& componentClassName)
{
	std::list<Component*> compsFound;
	for (Object* component : components)
	{
		if (component->GetDerivedClassName() == componentClassName)
		{
			compsFound.push_back(static_cast<Component*>(component));
		}
	}
	// If this ran before preUpdate, component would be in componentsToAdd
	for (Object* component : componentsToAdd)
	{
		if (component->GetDerivedClassName() == componentClassName)
		{
			compsFound.push_back(static_cast<Component*>(component));
		}
	}
	return compsFound;
}

bool Entity::RemoveComponent(const std::string& componentClassName)
{
	for (Object* component : components)
	{
		if (component->GetDerivedClassName() == componentClassName)
		{
			componentsToRemove.push_back(component);
			return true;
		}
	}
	return false;
}

bool Entity::RemoveComponent(Component* _component)
{
	for (Object* component : components)
	{
		if (component->GetUid() == _component->GetUid())
		{
			componentsToRemove.push_back(component);
			return true;
		}
	}
	return false;
}
