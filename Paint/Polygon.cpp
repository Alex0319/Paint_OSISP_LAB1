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
	free(polygonPoints);
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
	Polygon(hdc, polygonPoints, pointsCount);
}

void PolygonShape::SetPoint(POINTS point)
{
	if (startPoint.x == 0 && startPoint.y == 0)
		startPoint =endPoint= point;
	prevPointsCount = pointsCount++;
	polygonPoints = (POINT*)realloc(polygonPoints,pointsCount*sizeof(POINT));
	polygonPoints[pointsCount - 1].x = point.x;
	polygonPoints[pointsCount - 1].y = point.y;
}
