#include "stdafx.h"
#include "Engine/Systems/Engine.h"

extern void Engine_Register();

void Engine::Initialize()
{
	Engine_Register();
}

void Engine::Destroy()
{
}

void Engine::GameLoop()
{
}
