#include "stdafx.h"

EllipseShape::EllipseShape()
{
}

EllipseShape::EllipseShape(COLORREF color, int weight, COLORREF brushColor) :Shape(color, weight,brushColor)
{
}

EllipseShape::~EllipseShape()
{
}

void EllipseShape::Draw(HDC hdc,int weight)
{
	SelectObject(hdc, hBrush);
	SelectObject(hdc, hPen);
	if (pointsCount == 2)
		Ellipse(hdc, points[0].x, points[0].y, points[1].x, points[1].y);
}

void EllipseShape::SetPoint(POINTS point)
{
	if (pointsCount == 2)
	{
		points[1].x = point.x;
		points[1].y = point.y;
	}
	else
		Shape::SetPoint(point);
}
