// @file: Sprite.h
//
// @brief: Header file for Sprite class, responsible for loading and rendering a sprite.

#pragma once

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "Renderable.h"

class CSimpleSprite;
class Transform;

enum SPRITE_ANIM
{
	ANIM_FORWARDS,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT,
};

/**
 * @class Sprite
 *
 * This class is not really required because CSimpleSprite class represents a whole sprite entity by itself.
 * CSimpleSprite class has properties of a "Transform" and "Animator".
 * This class is still relevant because using CSimpleSprite by itself does not fit into my ECS design.
 * Hence, the Sprite class acts like a wrapper over CSimpleSprite. It'll use Transform and Animation data to
 * update the CSimpleSprite instance.
 */
class Sprite : public Renderable
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Sprite, Renderable)

	CSimpleSprite* csprite;
	SPRITE_ANIM currentAnimation = ANIM_FORWARDS;

	// Cached component to avoid reoccuring calls for its retreival from Entity
	Transform* transform = nullptr;

protected:
	Sprite() = default;
	~Sprite() = default;

	void Load(json::JSON&) override;
	void Initialize() override;
	void Update(float) override;
	void Render() override;
	void Destroy() override;

	// RenderSystem is going to call Render()
	friend class RenderSystem;
	// Sprite is part of an Entity
	friend class Entity;

public:
	SPRITE_ANIM GetAnimation() const { return currentAnimation; }
	void SetAnimation(SPRITE_ANIM _anim) { currentAnimation = _anim; }
};

#endif