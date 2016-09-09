#include "stdafx.h"

EllipseShape::EllipseShape()
{
}


EllipseShape::~EllipseShape()
{
}

void EllipseShape::Draw(HDC hdcPointer,HPEN pen)
{
	if (hPen == NULL)
		hPen = pen;
	SelectObject(hdcPointer, hPen);
	Ellipse(hdcPointer,startPoint.x,startPoint.y,endPoint.x,endPoint.y);
	SelectObject(hdcPointer, pen);
}

void EllipseShape::SetPoint(POINTS point)
{
	if (startPoint.x == 0 && startPoint.y == 0)
		startPoint = point;
	else
		endPoint = point;
}
