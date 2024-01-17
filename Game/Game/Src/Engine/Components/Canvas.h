// @file: Canvas.h
//
// @brief: Header file for Canvas class, responsible for rendering text on the screen.

#pragma once
#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "Engine/Components/Renderable.h"

struct CanvasText
{
	// Position
	float x = 0.0f, y = 0.0f;
	std::string text = "";

	// Color
	float r = 1.0f, g = 1.0f, b = 1.0f;

	CanvasText(float x, float y, std::string text, float r = 1.0f, float g = 1.0f, float b = 1.0f) :
		x(x), y(y), text(text), r(r), g(g), b(b) { }
};

class Canvas : public Renderable
{
	std::list<CanvasText> texts;

protected:
	// Protected destructor so that only Entity can delete it
	~Canvas() = default;

	void Initialize() override { }
	void Update(float) override { }
	void Render() override;
	void Destroy() override { }

public:
	Canvas() { type = CanvasC; }

	void AddText(const CanvasText&);
};


#endif // !_CANVAS_H_
