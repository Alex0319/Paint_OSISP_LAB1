#include "stdafx.h"

Line::Line()
{
}

Line::Line(COLORREF color, int weight, COLORREF brushColor) :Shape(color, weight,brushColor)
{
}

Line::~Line()
{
}

void Line::Draw(HDC hdc, int weight)
{
	SelectObject(hdc, hPen);
	MoveToEx(hdc, startPoint.x, startPoint.y, NULL);
	LineTo(hdc, endPoint.x, endPoint.y);
}

void Line::SetPoint(POINTS point)
{
	if (startPoint.x == 0 && startPoint.y == 0)
		startPoint =endPoint= point;
	else
		endPoint = point;
}
