#include "stdafx.h"

Pen::Pen()
{
}

Pen::Pen(COLORREF color, int weight, COLORREF brushColor) :PolygonLine(color, weight, brushColor)
{
}

Pen::~Pen()
{
}

void Pen::Draw(HDC hdc,int weight)
{
	SelectObject(hdc, hPen);
	Polyline(hdc, points, pointsCount);
}

void Pen::SetPoint(POINTS point)
{
	PolygonLine::SetPoint(point);
}
