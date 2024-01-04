// @file: ObjectPool.h
//
// @brief: Header file for ObjectPool, an abstract class for object pooling classes.

#pragma once
#ifndef _OBJECT_POOL_H_
#define _OBJECT_POOL_H_

class Object;

class ObjectPool
{
	const std::string CONFIG_FILE = "Assets/ObjectPoolConfig.json";

protected:
	int poolSize = 0;
	// If all objects in pool gets used up, its size gets increased by this amount
	int poolIncrementSize = 1;

	// Using vector due to O(1) access time
	std::vector<Object*> objects;

	// Storing free object indices in a stack
	// Using stack instead of queue for less cache miss
	std::stack<size_t> availableIndices;

	// For O(1) freeing time
	std::map<Object*, size_t> objectPtrToIndex;

	// Clean-up involves anything to-be-done before returning an object
	// to the pool or deleting it (at game quit)
	virtual void CleanUpObject(Object*) = 0;
	// Opposite of clean-up
	virtual void InitializeObjectForUse(Object*) = 0;

	virtual Object* CreateObjectForPool() = 0;

public:
	ObjectPool();
	~ObjectPool();

	Object* GetFreeObject();
	void MarkObjectAsFree(Object*);
};

#endif // !_OBJECT_POOL_H_
