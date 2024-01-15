// @file: ObjectPool.h
//
// @brief: Header file for ObjectPool, an abstract class for object pooling classes.

#pragma once
#ifndef _OBJECT_POOL_H_
#define _OBJECT_POOL_H_

class Object;

/**
 * @class ObjectPool
 *
 * Base class for implementing object pooling.
 */
class ObjectPool
{
protected:
	int poolSize = 10;
	// If all objects in pool gets used up, its size gets increased by this amount
	int poolIncrementSize = 5;

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

	// Any operation required before sending an object from pool to the game (opposite of cleanup)
	virtual void InitializeObject(Object*) = 0;
	
	// Any operation required right after creating an object
	virtual void SetupObject(Object*) = 0;

	// This method dynamically allocates the object. Dyanmic allocation is slow, hence this
	// method must be called as less as possible.
	// Object allocation initially happens at start of the game. Game developer must
	// create sufficiently big entity pool at that time. If all objects of the pool get used up,
	// the pool size gets increased in a batch. These configs are part of ObjectPoolConfig.json
	virtual Object* CreateObjectForPool() = 0;

public:
	ObjectPool() = default;

	Object* GetFreeObject();
	void MarkObjectAsFree(Object*);
};

#endif // !_OBJECT_POOL_H_
