#include "stdafx.h"

Shape::Shape()
{
}

Shape::Shape(COLORREF color, int weight, COLORREF brushColor)
{
	hPen = CreatePen(PS_SOLID, weight, color);
	hBrush = CreateSolidBrush(brushColor);
}

Shape::~Shape()
{
	DeleteObject(hPen);
	DeleteObject(hBrush);
}
