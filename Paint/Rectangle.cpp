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
	if (pointsCount == 2)
		Rectangle(hdc, points[0].x, points[0].y, points[1].x, points[1].y);
}

void RectangleShape::SetPoint(POINTS point)
{
	if (pointsCount == 2)
	{
		points[1].x = point.x;
		points[1].y = point.y;
	}
	else
		Shape::SetPoint(point);
}
