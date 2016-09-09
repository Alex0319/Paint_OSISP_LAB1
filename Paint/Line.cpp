#include "stdafx.h"

Line::Line()
{
}


Line::~Line()
{
}

void Line::Draw(HDC hdcPointer,HPEN pen)
{
	if (hPen == NULL)
		hPen = pen;
	SelectObject(hdcPointer, hPen);
	MoveToEx(hdcPointer, startPoint.x, startPoint.y, NULL);
	LineTo(hdcPointer, endPoint.x, endPoint.y);
	SelectObject(hdcPointer, pen);
}

void Line::SetPoint(POINTS point)
{
	if (startPoint.x == 0 && startPoint.y == 0)
		startPoint = point;
	else
		endPoint = point;
}
