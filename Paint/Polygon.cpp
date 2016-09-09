#include "stdafx.h"

Polygon::Polygon()
{
}


Polygon::~Polygon()
{
}

void Polygon::Draw(HDC hdcPointer, HPEN pen)
{
	if (hPen == NULL)
		hPen = pen;
	SelectObject(hdcPointer, hPen);
//	Ellipse(hdcPointer, startPoint.x, startPoint.y, endPoint.x - startPoint.x, endPoint.y - startPoint.y);
}

void Polygon::SetPoint(POINTS point)
{
	if (startPoint.x == 0 && startPoint.y == 0)
		startPoint = point;
	else
		endPoint = point;
}
