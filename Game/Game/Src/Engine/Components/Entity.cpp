// @file: Entity.cpp
//
// @brief: Cpp file for the Entity class. Entities hold Components.
//
// @author: Divyanshu N Singh (DNS)
// @date: 2023-12-03

#include "stdafx.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Component.h"
#include "Engine/Components/Transform.h"

IMPLEMENT_DYNAMIC_CLASS(Entity)

Entity::Entity()
{
	// Assign a default value to ensure we always have a transform
	transform = (Transform*)CreateComponent("Transform");
}

Entity::Entity(std::string _guid) : Object(_guid)
{
	// Assign a default value to ensure we always have a transform
	transform = (Transform*)CreateComponent("Transform");
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
			Component* component = CreateComponent(componentClassName);
			component->Load(componentJSON["ClassData"]);

			// Store transform separately too for easier / quicker access
			if (componentClassName == "Transform")
			{
				transform = (Transform*)component;
			}
			//LOG("Loaded up component " << componentClassName << " in entity " << name)
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

void Entity::Update()
{
	for (Object* component : components)
	{
		if (component->IsActive() && !SceneManager::Get().sceneChanged)
		{
			static_cast<Component*>(component)->Update();
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

json::JSON Entity::Save()
{
	json::JSON entityJSON;
	entityJSON["Name"] = GetName();
	entityJSON["GUID"] = GetGUID();

	json::JSON componentsJSON = json::JSON::Array();
	for (Object* component : components)
	{
		componentsJSON.append(component->Save());
	}
	entityJSON["Components"] = componentsJSON;
	return entityJSON;
}

void Entity::Destroy()
{
	// Transform is also one of the components. We don't have to delete it separately
	for (Object* component : components)
	{
		component->Destroy();
		delete component;
	}
	components.clear();
}

bool Entity::HasComponent(std::string& componentClassName)
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

Component* Entity::CreateComponent(std::string componentClassName)
{
	// Prevent creation of duplicate transform component
	if (componentClassName == "Transform" && HasComponent(componentClassName))
	{
		return GetComponent(componentClassName);
	}

	// DO NOT typecast to Component* here. Object slicing destroys derived class-specific information
	Object* component = CreateObject(componentClassName.c_str());
	componentsToAdd.push_back(component);
	static_cast<Component*>(component)->entity = this;
	return static_cast<Component*>(component);
}

Component* Entity::LoadComponent(std::string componentClassName, json::JSON& componentJSON)
{
	// Prevent creation of duplicate transform component
	if (componentClassName == "Transform" && HasComponent(componentClassName))
	{
		return GetComponent(componentClassName);
	}

	// DO NOT typecast to Component* here. Object slicing destroys derived class-specific information
	Object* component = CreateObject(componentClassName.c_str());
	static_cast<Component*>(component)->entity = this;
	component->Load(componentJSON);
	componentsToAdd.push_back(component);
	return static_cast<Component*>(component);
}

bool Entity::RemoveComponent(std::string& componentClassName)
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
