#include "stdafx.h"

PolygonShape::PolygonShape()
{
}

PolygonShape::PolygonShape(COLORREF color, int weight, COLORREF brushColor) :Shape(color, weight,brushColor)
{
	polygonPenColor = color;
	this->weight = weight;
	polygonBrushColor = brushColor;
}

PolygonShape::~PolygonShape()
{
}

void PolygonShape::SetBrush(HDC hdc)
{
	COLORREF currentBrushColor = GetDCBrushColor(hdc);
	if (polygonBrushColor != currentBrushColor && prevPointsCount < pointsCount)
	{
		DeleteObject(hBrush);
		polygonBrushColor = currentBrushColor;
		hBrush = CreateSolidBrush(polygonBrushColor);
	}
}

void PolygonShape::SetPen(HDC hdc,int weight)
{
	COLORREF currentColor = GetDCPenColor(hdc);
	if ((polygonPenColor != currentColor || this->weight!=weight) && prevPointsCount<pointsCount)
	{
		DeleteObject(hPen);
		polygonPenColor = currentColor;
		this->weight = weight;
		hPen = CreatePen(PS_SOLID, weight, polygonPenColor);
	}
	prevPointsCount++;
}

void PolygonShape::Draw(HDC hdc,int weight)
{
	SetBrush(hdc);
	SetPen(hdc,weight);
	SelectObject(hdc, hBrush);
	SelectObject(hdc, hPen);
	Polygon(hdc, points, pointsCount);
}

void PolygonShape::SetPoint(POINTS point)
{
	prevPointsCount = pointsCount;
	Shape::SetPoint(point);
}
