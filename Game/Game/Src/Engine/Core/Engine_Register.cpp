#include "stdafx.h"

#include "Engine/Core/Object.h"
#include "Engine/Components/Component.h"
#include "Engine/Components/Entity.h"
#include "Engine/Components/Transform.h"

// Registering type classes is important or else we won't be able to use TypeClass
void Engine_Register()
{
	REGISTER_TYPECLASS(Object)
	REGISTER_TYPECLASS(Component)
	REGISTER_TYPECLASS(Entity)
	REGISTER_TYPECLASS(Transform)
}
