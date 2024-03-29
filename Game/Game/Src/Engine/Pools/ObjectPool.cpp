// @file: ObjectPool.cpp
//
// @brief: Cpp file for ObjectPool, an abstract class for object pooling classes.

#include "stdafx.h"

#include "Engine/Pools/ObjectPool.h"
#include "Engine/Core/Object.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Scene.h"

Object* ObjectPool::GetFreeObject()
{
	if (availableIndices.empty())
	{
		// Increase pool size
		int t = poolIncrementSize;
		size_t idx = objects.size();
		while (t)
		{
			Object* object = CreateObjectForPool();
			objects.push_back(object);

			availableIndices.push(idx);
			objectPtrToIndex[object] = idx;

			--t; ++idx;
		}
	}

	size_t freeIndex = availableIndices.top();
	availableIndices.pop();

	Object* object = objects[freeIndex];
	object->SetActive(true);  // Not really required, but why not

	InitializeObject(object);

	return object;
}

void ObjectPool::MarkObjectAsFree(Object* object)
{
	if (objectPtrToIndex.find(object) == objectPtrToIndex.end())
	{
		return;
	}

	CleanUpObject(object);

	availableIndices.push(objectPtrToIndex[object]);
}
