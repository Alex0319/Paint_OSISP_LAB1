#include "stdafx.h"

Shape::Shape()
{
}

Shape::Shape(COLORREF color, int weight, COLORREF brushColor)
{
	hPen = CreatePen(PS_SOLID, weight, color);
	hBrush = CreateSolidBrush(brushColor);
}

Shape::~Shape()
{
	DeleteObject(hPen);
	DeleteObject(hBrush);
	free(points);
}

void Shape::RecalculateCoordinates(int deltaX, int deltaY)
{
	for (int i = 0; i < pointsCount; i++)
	{
		points[i].x += deltaX;
		points[i].y += deltaY;
	}
}

void Shape::SetPoint(POINTS point)
{
	POINT tmpPoint;
	tmpPoint.x = point.x;
	tmpPoint.y = point.y;
	pointsCount++;
	points = (POINT*)realloc(points, pointsCount*sizeof(POINT));
	points[pointsCount - 1].x = point.x;
	points[pointsCount - 1].y = point.y;
}
