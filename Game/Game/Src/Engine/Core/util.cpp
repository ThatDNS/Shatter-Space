// @file: util.cpp
//
// @brief: Cpp file for utility functions used throughout the project.

#include "stdafx.h"
#include "Engine/Core/util.h"
#include "Engine/Core/Logger.h"

// All components used in the game
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/Sprite.h"
#include "Game/Player.h"

STRCODE GetHashCode(const char* str)
{
	const unsigned int fnv_prime = 0x811C9DC5;
	unsigned int hash = 0;
	char end = '\0';

	for (const char* ref = str; *ref != end; ref++)
	{
		hash *= fnv_prime;
		hash ^= *ref;
	}

	return hash;
}

void CreateUUID(UUID* uid)
{
	UuidCreate(uid);
}

std::string GUIDTostring(UUID& guid)
{
	char guid_string[37];
	sprintf_s(
		guid_string,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	return guid_string;
}

STRCODE GUIDToSTRCODE(UUID& guid)
{
	return GetHashCode(GUIDTostring(guid).c_str());
}

Component* CreateComponent(ComponentType componentType)
{
	Component* component = nullptr;

	switch (componentType)
	{
	case TransformC:
		Logger::Get().Log("Trying to create a Transform on an entity. All entities have transform by default.", ERROR_LOG);
		break;
	case MeshRendererC:
		component = new MeshRenderer();
		break;
	case SpriteC:
		component = new Sprite();
		break;
	case PlayerC:
		component = new Player();
		break;
	default:
		Logger::Get().Log("Trying to create an invalid component on an entity.", ERROR_LOG);
	}
	return component;
}

std::string ComponentTypeToStr(ComponentType componentType)
{
	std::string componentName = "";

	switch (componentType)
	{
	case TransformC:
		componentName = "Transform";
		break;
	case MeshRendererC:
		componentName = "MeshRenderer";
		break;
	case SpriteC:
		componentName = "Sprite";
		break;
	case PlayerC:
		componentName = "Player";
		break;
	default:
		Logger::Get().Log("Trying to get string representation of invalid component.", ERROR_LOG);
	}

	return componentName;
}
