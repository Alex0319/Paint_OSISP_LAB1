#include "stdafx.h"

RectangleFactory::RectangleFactory()
{
}


RectangleFactory::~RectangleFactory()
{
}

Shape* RectangleFactory::CreateShape(COLORREF color, int weight, COLORREF brushColor)
{
	return new RectangleShape(color, weight, brushColor);
}
