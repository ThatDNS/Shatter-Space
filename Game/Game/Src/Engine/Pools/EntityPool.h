// @file: EntityPool.h
//
// @brief: Header file for EntityPool, a singleton class responsible which implements object pooling for entities.

#pragma once
#ifndef _ENTITY_POOL_H_
#define _ENTITY_POOL_H_

#include "Engine/Pools/ObjectPool.h"

class Entity;

/**
 * @class EntityPool
 *
 * EntityPool class implements object pooling for Entities.
 * 
 * It creates components of each entity with it for best cache coherence.
 * All the components of all entities in a scene get updated in one game loop.
 * Having these components tightly packed together in memory with the entity is essential for best performance.
 * This class ensures it.
 * 
 * There is a different EntityPool instance in SceneManager for each archetype of entity.
 * An archetype describes a type of entity, i.e. an entity having a particular type of components.
 */
class EntityPool : public ObjectPool
{
private:
	// Stores the components which are supposed to be part of the entities
	// present in this entity pool.
	// Component creation happens with new entity creates. Improves cache coherence.
	std::vector<std::string> componentClassNames;

	void CleanUpObject(Object*) override;
	Object* CreateObjectForPool() override;

	// Creates components of the entity passed as Object pointer.
	void InitializeObjectForUse(Object*) override;

	inline explicit EntityPool(EntityPool const&) = delete;
	inline EntityPool& operator=(EntityPool const&) = delete;

public:
	EntityPool(std::vector<std::string>&);
	~EntityPool();

	friend class Engine;
};

#endif // !_ENTITY_POOL_H_
