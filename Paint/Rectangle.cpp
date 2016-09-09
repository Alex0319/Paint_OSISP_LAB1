#include "stdafx.h"

RectangleShape::RectangleShape()
{
}


RectangleShape::~RectangleShape()
{
}

void RectangleShape::Draw(HDC hdcPointer, HPEN pen)
{
	if (hPen == NULL)
		hPen = pen;
	SelectObject(hdcPointer, hPen);

	Rectangle(hdcPointer, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
	SelectObject(hdcPointer, pen);
}

void RectangleShape::SetPoint(POINTS point)
{
	if (startPoint.x == 0 && startPoint.y == 0)
		startPoint = point;
	else
		endPoint = point;
}

