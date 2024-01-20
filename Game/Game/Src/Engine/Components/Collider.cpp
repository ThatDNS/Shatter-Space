// @file: Collider.cpp
//
// @brief: Cpp file for Collider class. Creating this component causes
// the entity to be tracked by collision system.

#include "stdafx.h"
#include "Engine/Components/Collider.h"

void Collider::OnCollisionEnter(Collider* other)
{
	if (OnCollisionEnterFunc != nullptr)
	{
		OnCollisionEnterFunc(other);
	}
}
