#pragma once

#ifndef _ENGINE_H_
#define _ENGINE_H_

class Engine
{
	DECLARE_SINGLETON(Engine)

public:
	void Initialize();
	void Destroy();
	void GameLoop();
};

#endif
