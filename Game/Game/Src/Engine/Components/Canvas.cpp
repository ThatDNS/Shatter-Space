#include "stdafx.h"
#include "Canvas.h"
#include "App/app.h"

void Canvas::AddText(const CanvasText& canvasText)
{
	texts.push_back(canvasText);
}

void Canvas::Render()
{
	for (CanvasText& cTxt : texts)
	{
		App::Print(cTxt.x, cTxt.y, cTxt.text.c_str(), cTxt.r, cTxt.g, cTxt.b);
	}
}
