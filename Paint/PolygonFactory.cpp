#include "stdafx.h"

PolygonFactory::PolygonFactory()
{
}


PolygonFactory::~PolygonFactory()
{
}

Shape* PolygonFactory::CreateShape(COLORREF color, int weight, COLORREF brushColor)
{
	return new PolygonShape(color, weight,brushColor);
}
