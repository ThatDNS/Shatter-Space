#pragma once

#ifndef _ENGINE_H_
#define _ENGINE_H_

class Engine
{
	DECLARE_SINGLETON(Engine)

public:
	/**
	 * @brief Anything that must be done before the level loads up 
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
