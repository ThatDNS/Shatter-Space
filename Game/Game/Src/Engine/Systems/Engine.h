#pragma once

#ifndef _ENGINE_H_
#define _ENGINE_H_

class Engine
{
	DECLARE_SINGLETON(Engine)
	
	// CollisionSystem gets updated once every COLLISION_SYSTEM_UPDATE_TIME ms
	float COLLISION_SYSTEM_UPDATE_TIME = 500.0f;

	float timeElapsed = 0.0f;

public:
	/**
	 * @brief Anything that must be done before the game loads up.
	 * Example, setting up the logger.
	 */
	void Wakeup();

	/**
	 * @brief Loads and Initializes all game systems.
	 * Any slow data-loading call must happen here.
	 */
	void Initialize();

	/**
	 * @brief Performs clean-up on all game systems.
	 */
	void Destroy();

	/**
	 * @brief Updates all game systems as per deltaTime.
	 */
	void Update(float deltaTime);

	/**
	 * @brief Renders entities.
	 */
	void Render();
};

#endif
