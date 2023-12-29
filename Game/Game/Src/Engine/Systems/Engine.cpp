#include "stdafx.h"
#include "Engine.h"

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
