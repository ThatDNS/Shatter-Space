#include "stdafx.h"

#include "Object.h"

// Registering type classes is important or else we won't be able to use TypeClass
void Engine_Register()
{
	REGISTER_TYPECLASS(Object)
}
