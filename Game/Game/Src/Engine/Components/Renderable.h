// @file: Renderable.h
//
// @brief: Header file for Renderable class.

#pragma once

#ifndef _Renderable_H_
#define _Renderable_H_

#include "Engine/Components/Component.h"

/**
 * @class Renderable
 *
 * Child classes of Renderable class automatically get tracked by RenderSystem.
 */
class Renderable : public Component
{
	DECLARE_ABSTRACT_DERIVED_CLASS(Renderable, Component)

protected:
	Renderable();
	~Renderable();

private:
	virtual void Render() = 0;

	friend class RenderSystem;
};

#endif

