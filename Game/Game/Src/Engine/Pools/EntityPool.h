// @file: EntityPool.h
//
// @brief: Header file for EntityPool, a singleton class responsible which implements object pooling for entities.

#pragma once
#ifndef _ENTITY_POOL_H_
#define _ENTITY_POOL_H_

#include "Engine/Pools/ObjectPool.h"

class Entity;

class EntityPool : public ObjectPool
{
public:
	static EntityPool& Get() {
		static EntityPool instance;
		return instance;
	}

private:
	void InitializeObjectForUse(Object*) override;
	void CleanUpObject(Object*) override;
	Object* CreateObjectForPool() override;

	inline explicit EntityPool(EntityPool const&) = delete;
	inline EntityPool& operator=(EntityPool const&) = delete;

public:
	EntityPool();
	~EntityPool();
};

#endif // !_ENTITY_POOL_H_
