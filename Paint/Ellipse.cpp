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
	Ellipse(hdc,startPoint.x,startPoint.y,endPoint.x,endPoint.y);
}

void EllipseShape::SetPoint(POINTS point)
{
	if (startPoint.x == 0 && startPoint.y == 0)
		startPoint =endPoint= point;
	else
		endPoint = point;
}
