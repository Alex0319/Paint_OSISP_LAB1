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
	if (pointsCount == 2)
	{
		MoveToEx(hdc, points[0].x, points[0].y, NULL);
		LineTo(hdc, points[1].x, points[1].y);
	}
}

void Line::SetPoint(POINTS point)
{
	if (pointsCount == 2)
	{
		points[1].x = point.x;
		points[1].y = point.y;
	}
	else
		Shape::SetPoint(point);
}
