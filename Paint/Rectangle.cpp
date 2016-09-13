#include "stdafx.h"

RectangleShape::RectangleShape()
{
}

RectangleShape::RectangleShape(COLORREF color, int weight, COLORREF brushColor) :Shape(color, weight,brushColor)
{
}

RectangleShape::~RectangleShape()
{
}

void RectangleShape::Draw(HDC hdc,int weight)
{
	SelectObject(hdc,hPen);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
}

void RectangleShape::SetPoint(POINTS point)
{
	if (startPoint.x == 0 && startPoint.y == 0)
		startPoint =endPoint= point;
	else
		endPoint = point;
}
